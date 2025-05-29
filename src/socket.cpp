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
		
			close(clientSocket);
		}).detach();
	}
}

void Socket::initSocket() {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(2929);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	listen(serverSocket, 5);
	
	Socket::startAcceptingClients(serverSocket);
}
