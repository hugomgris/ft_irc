/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:38:22 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/14 15:41:11 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/SocketManager.hpp"
#include <stdexcept>

//Constructor and destructor
SocketManager::SocketManager(int port): _port(port), _server_fd(-1), _client_fd(-1) {
	if (!createSocket() || !bindSocket() || !listenForConnections()) {
		throw SocketFailureException();
	}
}

SocketManager::~SocketManager() {
	if (_server_fd != -1) {
        close(_server_fd);
    }
}

//Getters and Setters
int SocketManager::getServerFd() const{return (_server_fd);}
int SocketManager::getClientFd() const{return (_client_fd);}
void SocketManager::setClientFd(int &client_fd) {_client_fd = client_fd;}

//Methods
bool SocketManager::createSocket() {
	//Create socket
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd == -1) {
		std::cerr << "Socket creation failed" << std::endl;
		return (false);
	}

	//Set socket options to reuse address (avoids already in use errors)
	int opt = 1;
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		std::cerr << "Setsockopt failed" << std::endl;
		close(_server_fd);
		return (false);
	}

	//Define server address
	std::memset(&_server_addr, 0, sizeof(_server_addr));
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = INADDR_ANY;
	_server_addr.sin_port = htons(_port);

	return (true);
}

bool SocketManager::bindSocket() {
	if (bind(_server_fd, (struct sockaddr*)&_server_addr, sizeof(_server_addr)) == -1) {
		std::cerr << "Bind failed" << std::endl;
		close(_server_fd);
		return (false);
	}

	return (true);
}

bool SocketManager::listenForConnections() {
	if (listen(_server_fd, 5) == -1) {
		std::cerr << "Listen failed" << std::endl;
		close(_server_fd);
		return (false);
	}
	std::cout << "Server listening on port " << _port << std::endl;
	return (true);
}

int SocketManager::acceptConnection() {
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	int client_fd = accept(_server_fd, (struct sockaddr*)&client_addr, &client_len);
	
	if (client_fd == -1)
		std::cerr << "Accept failed" << std::endl;

	return (client_fd);
}

void SocketManager::closeSocket(int client_fd) {
	if (client_fd != -1) {
		close(client_fd);
		std::cout << "Client disconnected." << std::endl;
	}
}

void SocketManager::pollForClients() {
	
}

//Exception messages
const char *SocketManager::SocketFailureException::what() const throw() {
	return ("Socket error while creating or setting up server socket");
}