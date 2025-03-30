/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:08:39 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/27 15:35:03 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerManager.hpp"

// Message macros
const std::string ServerManager::REGISTERERR = "Error registering user";
const std::string ServerManager::REGISTERSCCS = "User successfully registered with nickname ";
const std::string ServerManager::WELCOMEMSG = "Welcome to ft_irc!";
const std::string ServerManager::RECONECTSCCS = "Welcome back to ft_irc";

//Constructor and destructor
ServerManager::ServerManager(int port, const std::string &password) 
	: _serverName("ft_irc"), 
	_socketManager(new SocketManager(port)), 
	_clientManager(new ClientManager(password, this)),
	_channelManager(new ChannelManager(_clientManager)), 
	_commandManager(new CommandManager(_clientManager, _channelManager, this)) {
}

ServerManager::~ServerManager() {
    delete _socketManager;
    delete _clientManager;
    delete _commandManager;
}

// Getters
ClientManager *ServerManager::getClientManager(){
    return (_clientManager);
}

ChannelManager *ServerManager::getChannelManager() {
    return (_channelManager);
}

std::string &ServerManager::getServerName() {
    return (_serverName);
}

// Methods
void ServerManager::run() {
    struct pollfd server_poll_fd;
    server_poll_fd.fd = _socketManager->getServerFd();
    server_poll_fd.events = POLLIN;
    _fds.push_back(server_poll_fd);

    time_t lastPingCheckTime = time(NULL);
	time_t lastBotMessageTime = time(NULL);


    while (true) {
        int poll_count = poll(&_fds[0], static_cast<unsigned int>(_fds.size()), 1000);
        if (poll_count < 0) {
            std::cerr << "Poll error." << std::endl;
            break;
        }


        for (size_t i = 0; i < _fds.size(); ++i) {
            if (_fds[i].fd == _socketManager->getServerFd() && (_fds[i].revents & POLLIN)) {
                int client_fd = _socketManager->acceptConnection();
                if (client_fd != -1) {
                    std::cout << "Client connected (fd:" << client_fd << ")" << std::endl;
                    struct pollfd client_poll_fd = {client_fd, POLLIN | POLLHUP, 0};
                    _fds.push_back(client_poll_fd);
                    
                    ClientBuffer *clientBuffer = new ClientBuffer(client_fd);
                    addClientBuffer(client_fd, clientBuffer);

                    RegisteredClient* client = _clientManager->getClientFromFd(client_fd);
                    if (client)
                        client->setLastPongTime(time(NULL));
                }
            } else if (_fds[i].revents & POLLIN) {
                std::string input;
                if (!readFromClient(_fds[i].fd, input) && getClientBufferFromFd(_fds[i].fd)->getBuffer().empty()) {
                    std::cout << "Client disconnected (fd: " << _fds[i].fd << ")" << std::endl;
                    _clientManager->setClientOffline(_fds[i].fd);
                    _clientManager->clearPendingRegistration(_fds[i].fd);
                    close(_fds[i].fd);
                    _fds.erase(_fds.begin() + i);
                    --i;
                } else {
                    _commandManager->executeCommand(_fds[i].fd, input);
                    
                    //!Erase this before turning in
                    std::cout << "Client sent:" << input << std::endl;

                    RegisteredClient* client = _clientManager->getClientFromFd(_fds[i].fd);
                    if (client)
                        client->setLastPongTime(time(NULL));
                }
            }
        }

        // **PING/PONG Handling**
        time_t currentTime = time(NULL);
        if (currentTime - lastPingCheckTime >= 30) { // Every 30 seconds
            std::cout << "Sending PING" << std::endl;
            sendPingToClients();
            checkClientTimeouts();
            lastPingCheckTime = currentTime;
        }

		if (currentTime - lastBotMessageTime >= 60) {
			for (std::map<std::string, Channel>::const_iterator it = _channelManager->getChannelList().begin(); it != _channelManager->getChannelList().end(); ++it) {
					
					RegisteredClient* bot = _clientManager->getClientFromFd(1000);
					std::string message = "PRIVMSG " + it->second.getName() + " : **you are currently evaluating a 125/125 ft_irc**";
					_commandManager->executeCommand(bot->getFd(), message);
				}
			lastBotMessageTime = currentTime;
			}
		}

    }

bool ServerManager::readFromClient(int client_fd, std::string &input) {
    char buffer[1024];
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_read <= 0) {
        getClientBufferFromFd(client_fd)->getBuffer().erase();
        return false;
    }

    buffer[bytes_read] = '\0';
    std::string tmpBuffer = buffer;
    
    if (tmpBuffer.length() >= 2 && tmpBuffer[tmpBuffer.length() - 2] == '\r' && tmpBuffer[tmpBuffer.length() - 1] == '\n') {
        if (!getClientBufferFromFd(client_fd)->getBuffer().empty()) {
            input = getClientBufferFromFd(client_fd)->getBuffer();
        }
        input += tmpBuffer.substr(0, tmpBuffer.length() - 2);
        getClientBufferFromFd(client_fd)->getBuffer().erase();
    }
    else if (tmpBuffer.length() >= 1 && tmpBuffer[tmpBuffer.length() - 1] == '\n') {
        if (!getClientBufferFromFd(client_fd)->getBuffer().empty()) {
            input = getClientBufferFromFd(client_fd)->getBuffer();
        }
        input += tmpBuffer.substr(0, tmpBuffer.length() - 1);
        getClientBufferFromFd(client_fd)->getBuffer().erase();
    }
    else {
        getClientBufferFromFd(client_fd)->appendToBuffer(buffer);
        input.erase();
        return (false);
    }
    
    return true;
}

void ServerManager::disconnectClient(const std::string &nickname, int client_fd) {
	const std::map<std::string, RegisteredClient>& clientList = _clientManager->getRegisteredClients();
	std::map<std::string, RegisteredClient>::const_iterator it = clientList.find(nickname);

	if (it != clientList.end()) {
		if (it->second.getFd() == client_fd) {
            std::cout << "Client " << nickname << " disconnected." << std::endl;
			_clientManager->unregisterClient(nickname, client_fd);
			
			// Close the socket
			close(client_fd);
			
			for (size_t i = 0; i < _fds.size(); ++i) {
				if (_fds[i].fd == client_fd) {
					_fds.erase(_fds.begin() + i);
					break;
				}
			}

            ClientBuffer *clientBuffer = getClientBufferFromFd(client_fd);
            if (clientBuffer) {
                _clientBuffers.erase(client_fd);
                delete clientBuffer;
            }
		} else {
			std::cerr << "Provided fd does not match the client's registered fd." << std::endl;
		}
	} else {
		std::cerr << "Client " << nickname << " not found in the registered clients list." << std::endl;
	}
}

void ServerManager::sendPingToClients() {
    time_t now = time(NULL);
    std::ostringstream oss;
    oss << now;
    std::string token = oss.str();

    for (std::map<int, RegisteredClient>::iterator it = _clientManager->getRegisteredFds().begin(); it != _clientManager->getRegisteredFds().end(); ++it) {
        int client_fd = it->first;
        std::string pingMsg = "PING :" + token + "\r\n";
        send(client_fd, pingMsg.c_str(), pingMsg.length(), 0);
        it->second.setLastPingTime(now);
    }
}

void ServerManager::checkClientTimeouts() {
	time_t now = time(NULL);

	std::map<int, RegisteredClient>& clients = _clientManager->getRegisteredFds();

	for (std::map<int, RegisteredClient>::iterator it = clients.begin(); it != clients.end();) {
		std::map<int, RegisteredClient>::iterator next = it;
		++next;

        if (now - it->second.getLastPongTime() > 90) {
			std::cout << "Client " << it->second.getNickname() << " timed out.\n";
            std::string timedOutMessage = "Disconnected (timed out)\r\n";
            send(it->second.getFd(), timedOutMessage.c_str(), timedOutMessage.length(), 0);
			_commandManager->executeCommand(it->second.getFd(), "QUIT :timed out");
		}
		it = next;
	}
}

// Buffer related methods
void ServerManager::addClientBuffer(int client_fd, ClientBuffer *clientBuffer) {
    _clientBuffers[client_fd] = clientBuffer;
}

void ServerManager::removeClientBuffer(int client_fd) {
    _clientBuffers.erase(client_fd);
}

ClientBuffer *ServerManager::getClientBufferFromFd(int client_fd) {
    return (_clientBuffers[client_fd]);
}