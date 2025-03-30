/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientBuffer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:29:06 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/26 11:49:46 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/objects/ClientBuffer.hpp"

ClientBuffer::ClientBuffer() : _buffer(""), _fd(-1){}

ClientBuffer::ClientBuffer(int client_fd) : _buffer(""), _fd(client_fd){}

ClientBuffer::~ClientBuffer() {}

std::string &ClientBuffer::getBuffer() {
    return _buffer;
}

int ClientBuffer::getFd() const {
    return _fd;
}

void ClientBuffer::setFd(int fd) {
    _fd = fd;
}

void ClientBuffer::appendToBuffer(const std::string& data) {
    _buffer += data;
}

void ClientBuffer::clearBuffer() {
    _buffer.clear();
}