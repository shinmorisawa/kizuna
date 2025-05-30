#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <thread>
#include "http.hpp"
#include "socket.hpp"
#include "app.hpp"
#include "openssl/ssl.h"
#include "openssl/err.h"

void Socket::startAcceptingClients(int serverSocket) {
	while (true) {	
		sockaddr_in clientAddr;
		socklen_t clientLen = sizeof(clientAddr);		
		int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
		
		std::thread([clientSocket, clientAddr]() {
			char ip[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &(clientAddr.sin_addr), ip, INET_ADDRSTRLEN);
		
			char buffer[16384];
			recv(clientSocket, buffer, sizeof(buffer), 0);
			std::string request(buffer);
			
			HTTP::Response response;
			HTTP::Request parsed_request = HTTP::parseRequest(request);
			parsed_request.ip = ip;
			if (parsed_request.method == "GET") { response = App::returnResponse(parsed_request); }
			if (parsed_request.method == "POST") {
				int size = std::stoi(parsed_request.headers["Content-Length"]);
				int bytesReceived = 0;
				int bytesRead = 0;
		
				while (bytesReceived < size) {
					bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
					std::cout << bytesRead << std::endl;
					if (bytesRead <= 0) break;
					parsed_request.body.append(buffer, bytesRead);
					bytesReceived += bytesRead;
					std::cout << bytesReceived << std::endl;
				}
				
				parsed_request.body.shrink_to_fit();
				App::handlePost(parsed_request);
				response = App::returnResponse(parsed_request);
			}
		
			std::string raw = response.toString();

			send(clientSocket, raw.c_str(), raw.size(), 0);

			std::string().swap(raw);
		
			close(clientSocket);
		}).detach();
	}
}

void Socket::startAcceptingTLSClients(SSL_CTX* ctx, int serverSocket) {
	while (true) {	
		sockaddr_in clientAddr;
		socklen_t clientLen = sizeof(clientAddr);		
		int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
		
		std::thread([clientSocket, clientAddr, ctx]() {
			char ip[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &(clientAddr.sin_addr), ip, INET_ADDRSTRLEN);
		
			SSL *ssl = SSL_new(ctx);
			SSL_set_fd(ssl, clientSocket);

			if (SSL_accept(ssl) <= 0) {
				ERR_print_errors_fp(stderr);
				SSL_free(ssl);
				close(clientSocket);
				return;
			}

			char buffer[16384];
			SSL_read(ssl, buffer, sizeof(buffer));
			std::string request(buffer);
			
			HTTP::Response response;
			HTTP::Request parsed_request = HTTP::parseRequest(request);
			parsed_request.ip = ip;
			if (parsed_request.method == "GET") { response = App::returnResponse(parsed_request); }
			if (parsed_request.method == "POST") {
				int size = std::stoi(parsed_request.headers["Content-Length"]);
				int bytesReceived = 0;
				int bytesRead = 0;
		
				while (bytesReceived < size) {
					bytesRead = SSL_read(ssl, buffer, sizeof(buffer));
					std::cout << bytesRead << std::endl;
					if (bytesRead <= 0) break;
					parsed_request.body.append(buffer, bytesRead);
					bytesReceived += bytesRead;
					std::cout << bytesReceived << std::endl;
				}
				
				parsed_request.body.shrink_to_fit();
				App::handlePost(parsed_request);
				response = App::returnResponse(parsed_request);
			}
		
			std::string raw = response.toString();

			SSL_write(ssl, raw.c_str(), raw.size());

			std::string().swap(raw);
		
			SSL_shutdown(ssl);
			SSL_free(ssl);
			close(clientSocket);
		}).detach();
	}
}

void Socket::initSocket() {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(80);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	listen(serverSocket, 5);
	
	Socket::startAcceptingClients(serverSocket);
}

void Socket::initTLSSocket() {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(443);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	listen(serverSocket, 5);

	SSL_CTX* ctx = createContext();
	configureContext(ctx);

	Socket::startAcceptingTLSClients(ctx, serverSocket);
}

SSL_CTX* Socket::createContext() {
	const SSL_METHOD* method = TLS_server_method();
	SSL_CTX* ctx = SSL_CTX_new(method);
	if (!ctx) {
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
	return ctx;
}

void Socket::configureContext(SSL_CTX* ctx) {
	if (SSL_CTX_use_certificate_file(ctx, "/etc/kizuna/ssl/cert.pem", SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}

	if (SSL_CTX_use_PrivateKey_file(ctx, "/etc/kizuna/ssl/key.pem", SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
}

