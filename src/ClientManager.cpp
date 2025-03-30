/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:20:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/26 14:42:47 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerManager.hpp"
#include "../includes/ClientManager.hpp"
#include "../includes/ChannelManager.hpp"
#include "../includes/objects/Channel.hpp"

// Constructor and destructor
ClientManager::ClientManager(const std::string &password, ServerManager *serverManager): _password(password), _serverAddress("localhost"), _serverManager(serverManager) {}

ClientManager::~ClientManager() {}

//Getters and setters
std::map<std::string, RegisteredClient> &ClientManager::getRegisteredClients(void){
    return (_registeredClients);
}


std::map<int, RegisteredClient> &ClientManager::getRegisteredFds(void){
    return (_registeredFds);
}

RegisteredClient *ClientManager::getClientFromNickname(std::string &nickname) {
    if (_registeredClients.find(nickname) != _registeredClients.end()) {
        return &_registeredClients[nickname];
    }
    return NULL;
}

RegisteredClient *ClientManager::getClientFromFd(int fd) {
    if (_registeredFds.find(fd) != _registeredFds.end()) {
        return &_registeredFds[fd];
    }
    return NULL;
}


// Methods
bool ClientManager::isNicknameRegistered(const std::string& nickname) const {
    return _registeredClients.find(nickname) != _registeredClients.end();
}

bool ClientManager::isRegistered(std::string &nickname, int client_fd) {
    if (_registeredClients.find(nickname) != _registeredClients.end()) {
        std::cerr << "Client " << nickname << " is already registered." << std::endl;
        if (_registeredClients[nickname].getFd() != client_fd)
            _registeredClients[nickname].setFd(client_fd);
        return true;
    }
    return false;
}

bool ClientManager::registerClient(const std::string &nickname, const std::string &username, int client_fd) {
    // Register the client
    _registeredClients[nickname] = RegisteredClient(client_fd, nickname, username);
    _registeredFds[client_fd] = RegisteredClient(client_fd, nickname, username);

    std::cout << "Client " << client_fd << " registered with nickname: " << nickname << " and username: " << username << std::endl;
    
    std::string welcomeMessage = ":" + _serverAddress + " 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "! (" + username + "@" + "localhost)\r\n";
    
    ssize_t bytes_sent = send(client_fd, welcomeMessage.c_str(), welcomeMessage.size(), 0);
    
    if (bytes_sent == -1) {
        std::cerr << "Error sending welcome message to client " << client_fd << std::endl;
        return false;
    }

    // Success
    return true;
}

void ClientManager::setClientOffline(int fd) {
    for (std::map<std::string, RegisteredClient>::iterator it = _registeredClients.begin(); it != _registeredClients.end(); ++it) {
        if (it->second.getFd() == fd) {
            std::cout << "Setting client offline: " << it->first << " (fd: " << fd << ")" << std::endl;
            it->second.setFd(-1);
            it->second.setOnline(false);
            break ;
        }
    }

    std::map<int, RegisteredClient>::iterator it = _registeredFds.find(fd);
    if (it != _registeredFds.end()) {
        _registeredFds.erase(it);
    }
}

bool ClientManager::reconnectClient(const std::string& nickname, int new_fd) {
    if (_registeredClients.find(nickname) != _registeredClients.end()) {
        if (_registeredClients[nickname].isOnline() && _registeredClients[nickname].getFd() != -1) {
            std::cerr << "Client " << nickname << " is already connected with a different fd." << std::endl;
            return false;
        }

        _registeredClients[nickname].setFd(new_fd);
        _registeredClients[nickname].setOnline(true);

        if (_registeredFds.find(new_fd) != _registeredFds.end()) {
            _registeredFds[new_fd] = _registeredClients[nickname];
        } else {
            _registeredFds[new_fd] = _registeredClients[nickname];
        }

        std::cout << "Client " << nickname << " reconnected with fd: " << new_fd << std::endl;
        return true;
    }
    return false;
}

void ClientManager::unregisterClient(const std::string &nickname, int client_fd) {
    (void)client_fd;
    std::map<std::string, RegisteredClient>::iterator it = _registeredClients.find(nickname);
    if (it != _registeredClients.end()) {
        int fd = it->second.getFd();
        
        ChannelManager *channelManager = _serverManager->getChannelManager();
        if (channelManager) {
            channelManager->removeClientFromAllInviteLists(getClientFromFd(client_fd));
        }
        
        std::map<int, RegisteredClient>::iterator fdIt = _registeredFds.find(fd);
        if (fdIt != _registeredFds.end()) {
            _registeredFds.erase(fdIt);
        }

        _registeredClients.erase(it);
    }
}

void ClientManager::addPendingPassword(int fd, const std::string& password) {
    _pendingPasswords[fd] = password;
    std::cout << "Added pending password for fd: " << fd << std::endl;
}

void ClientManager::addPendingNickname(int fd, const std::string& nickname) {
    _pendingNicknames[fd] = nickname;
    std::cout << "Added pending nickname for fd: " << fd << ": " << nickname << std::endl;
}

void ClientManager::addPendingUsername(int fd, const std::string& username) {
    _pendingUsernames[fd] = username;
    std::cout << "Added pending username for fd: " << fd << ": " << username << std::endl;
}

bool ClientManager::isPendingRegistrationComplete(int fd) {
    return _pendingPasswords.find(fd) != _pendingPasswords.end() &&
           _pendingNicknames.find(fd) != _pendingNicknames.end() &&
           _pendingUsernames.find(fd) != _pendingUsernames.end();
}

void ClientManager::completePendingRegistration(int fd) {
    if (isPendingRegistrationComplete(fd)) {
        std::string password = _pendingPasswords[fd];
        std::string nickname = _pendingNicknames[fd];
        std::string username = _pendingUsernames[fd];
        
        if (password != _password) {
            std::cout << "Password verification failed for fd: " << fd << std::endl;
            return;
        }
        
        if (isNicknameRegistered(nickname)) {
            std::cout << "Nickname already registered: " << nickname << std::endl;
            return;
        }
        
        registerClient(nickname, username, fd);
        
        clearPendingRegistration(fd);
        
        std::cout << "Registration complete for fd: " << fd << std::endl;
    } else {
        std::cout << "Cannot complete registration, missing information for fd: " << fd << std::endl;
    }
}

void ClientManager::clearPendingRegistration(int fd) {
    _pendingPasswords.erase(fd);
    _pendingNicknames.erase(fd);
    _pendingUsernames.erase(fd);
}

bool ClientManager::hasPendingPassword(int fd) {
    return _pendingPasswords.find(fd) != _pendingPasswords.end();
}

bool ClientManager::hasPendingNickname(int fd) {
    return _pendingNicknames.find(fd) != _pendingNicknames.end();
}

bool ClientManager::hasPendingUsername(int fd) {
    return _pendingUsernames.find(fd) != _pendingUsernames.end();
}

std::string ClientManager::getPendingNickname(int fd) {
    if (hasPendingNickname(fd)) {
        return _pendingNicknames[fd];
    }
    return "";
}

// Testing methods
void ClientManager::printClientList(const int &client_fd) const {
    for (std::map<std::string, RegisteredClient>::const_iterator it = _registeredClients.begin(); it != _registeredClients.end(); ++it) {
		std::string status = it->second.isOnline() ? "online" : "offline";
		
		std::ostringstream oss;
		oss << it->second.getFd();
		std::string fdStr = oss.str();

		std::string line = it->second.getNickname() + " " + it->second.getUsername() + "-" + fdStr + "-" + status + "\r\n";
		send(client_fd, line.c_str(), line.length(), 0);
    }
}