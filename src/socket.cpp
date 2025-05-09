#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <thread>
#include "parse.hpp"
#include "socket.hpp"

void Socket::startAcceptingClients(int serverSocket) {
	while (true) {
		int clientSocket = accept(serverSocket, nullptr, nullptr);
		
		std::thread([clientSocket]() {
			char buffer[16384];

			recv(clientSocket, buffer, sizeof(buffer), 0);
	
			std::string request(buffer);
			std::cout << request;

			HTTP::parseRequest(request);

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
