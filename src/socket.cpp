#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <thread>
#include "http.hpp"
#include "socket.hpp"
#include "app.hpp"

void Socket::startAcceptingClients(int serverSocket) {
	while (true) {
		int clientSocket = accept(serverSocket, nullptr, nullptr);
		if (clientSocket < 0) continue;

		std::thread([clientSocket]() {
			char buffer[16384];

			recv(clientSocket, buffer, sizeof(buffer), 0);
			std::string request(buffer);

			HTTP::Request parsed_request = HTTP::parseRequest(request);
			HTTP::Response response = App::returnResponse(parsed_request);
			std::string raw = response.toString();

			send(clientSocket, raw.c_str(), sizeof(raw.c_str()), 0);

			close(clientSocket);
		}).detach();
	}
}

void Socket::initSocket() {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(20220);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	listen(serverSocket, 5);
	
	Socket::startAcceptingClients(serverSocket);
}
