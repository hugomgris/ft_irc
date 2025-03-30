/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:16:37 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/26 14:40:30 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTMANAGER_HPP
# define CLIENTMANAGER_HPP

# include <map>
# include <vector>
# include <string>
# include <iostream>
# include <netinet/in.h>
# include <sstream>

# include "objects/RegisteredClient.hpp"

class Channel;
class ServerManager;

class ClientManager {
private:
	std::string _password;
	std::string _serverAddress;
	ServerManager *_serverManager;
	
	std::map<std::string, RegisteredClient> _registeredClients;
	std::map<int, RegisteredClient> _registeredFds;
	
	// New members for pending registration
	std::map<int, std::string> _pendingPasswords;
	std::map<int, std::string> _pendingNicknames;
	std::map<int, std::string> _pendingUsernames;

	// Coplien stuff
	ClientManager();
	ClientManager(const ClientManager &other);
	ClientManager &operator=(const ClientManager &other);

public:
	// Constructor and destructor
	ClientManager(const std::string &password, ServerManager *serverManager);
	~ClientManager();

	// Getters and setters
	std::map<std::string, RegisteredClient> &getRegisteredClients(void);
	std::map<int, RegisteredClient> &getRegisteredFds(void);
	RegisteredClient *getClientFromNickname(std::string &nickname);
	RegisteredClient *getClientFromFd(int fd);

	// Methods
	bool isNicknameRegistered(const std::string& nickname) const;
	bool isRegistered(std::string &nickname, int client_fd);
	bool registerClient(const std::string &nickname, const std::string &username, int client_fd);
	void setClientOffline(int fd);
	bool reconnectClient(const std::string &nickname, int new_fd);
	void unregisterClient(const std::string &nickname, int client_fd);
	
	// Handling of IRC protocol registration
	void addPendingPassword(int fd, const std::string& password);
	void addPendingNickname(int fd, const std::string& nickname);
	void addPendingUsername(int fd, const std::string& username);
	bool isPendingRegistrationComplete(int fd);
	void completePendingRegistration(int fd);
	void clearPendingRegistration(int fd);
	bool hasPendingPassword(int fd);
	bool hasPendingNickname(int fd);
	bool hasPendingUsername(int fd);
	std::string getPendingNickname(int fd);

	//! Testing stuff
	void printClientList(const int &client_fd) const;
};

#endif