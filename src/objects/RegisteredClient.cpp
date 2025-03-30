/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RegisteredClient.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:17:54 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/26 14:01:25 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/objects/RegisteredClient.hpp"

RegisteredClient::RegisteredClient() : _fd(-1), _nickname(""), _username(""), _online(false) {}

RegisteredClient::RegisteredClient(int fd, const std::string &nickname, const std::string &username) 
    : _fd(fd), _nickname(nickname), _username(username), _online(true), _isBot(false) {}

RegisteredClient::~RegisteredClient() {}

// Getters and Setters
int RegisteredClient::getFd() const {
    return _fd;
}

const std::string &RegisteredClient::getNickname() const {
    return _nickname;
}

const std::string &RegisteredClient::getUsername() const {
    return _username;
}

const std::string &RegisteredClient::getRealName() const {
    return (_realName);
}

bool RegisteredClient::isOnline() const {
    return _online;
}

bool RegisteredClient::isBot() const {
    return _isBot;
}

void RegisteredClient::setFd(int fd) {
    _fd = fd;
}

void RegisteredClient::setNickname(const std::string &nickname) {
    _nickname = nickname;
}

void RegisteredClient::setUsername(const std::string &username) {
    _username = username;
}

void RegisteredClient::setOnline(bool status) {
    _online = status;
}

void RegisteredClient::setBot(bool status) {
    _isBot = status;
}

void RegisteredClient::setLastPingTime(time_t time) {
    _lastPingTime = time;
}

void RegisteredClient::setLastPongTime(time_t time) {
    _lastPongTime = time;
}

time_t RegisteredClient::getLastPingTime() const {
    return _lastPingTime;
}

time_t RegisteredClient::getLastPongTime() const {
    return _lastPongTime;
}

std::string &RegisteredClient::getBuffer() {
    return _buffer;
}

//Methods
std::string RegisteredClient::getHost() const {
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);

	if (getpeername(_fd, (struct sockaddr*)&addr, &addr_len) == -1) {
		return "UNKNOWN"; 
	}

	const char *ipStr = inet_ntoa(addr.sin_addr);

	return std::string(ipStr);
}

void RegisteredClient::appendToBuffer(const std::string& data) {
    _buffer += data;
}