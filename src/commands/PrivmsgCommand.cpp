/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:24:24 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/27 11:27:28 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/PrivmsgCommand.hpp"
#include "../../includes/ClientManager.hpp"
#include "../../includes/ChannelManager.hpp"

// Constructor and Destructor
PrivmsgCommand::PrivmsgCommand(ChannelManager* channelManager, ClientManager* clientManager): _channelManager(channelManager), _clientManager(clientManager) {}

PrivmsgCommand::~PrivmsgCommand() {}

// Methods
void PrivmsgCommand::executeCommand(int client_fd, const ParsedMessage& parsedMsg) {
	if (parsedMsg.params.size() < 2) {
		std::string errorMsg = "ERROR: PRIVMSG requires a target and a message\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	std::string target = parsedMsg.params[0];
	std::string message = parsedMsg.params[1];

	RegisteredClient* sender = _clientManager->getClientFromFd(client_fd);
	if (!sender) {
		std::string errorMsg = "ERROR: You must be a registered client to send messages\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	if (message.find("DCC SEND") == 0) {
		std::cout << "📁 DCC SEND request detected from " << sender->getNickname() << " to " << target << ": " << message << std::endl;
	}

	if (target[0] == '#') {
		std::cout << "Target:" << target << std::endl;
		Channel* channel = _channelManager->getChannel(target);
		if (!channel) {
			std::string errorMsg = "ERROR: No such channel: " + target + "\r\n";
			send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
			return;
		}

		if (!channel->isMember(sender)) {
			std::string errorMsg = "ERROR: You must join the channel before sending messages\r\n";
			send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
			return;
		}
		broadcastMessage(message, channel, sender);
	} else {
		RegisteredClient* recipient = _clientManager->getClientFromNickname(target);
		if (!recipient) {
			std::string errorMsg = "ERROR: No such user: " + target + "\r\n";
			send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
			return;
		}

		sendMessageToUser(message, recipient, sender);
	}
}

void PrivmsgCommand::sendMessageToUser(const std::string& message, RegisteredClient* recipient, RegisteredClient* sender) {
    std::string formattedMessage = ":" + sender->getNickname() + " PRIVMSG " + recipient->getNickname() + " :" + message + "\r\n";

    send(recipient->getFd(), formattedMessage.c_str(), formattedMessage.length(), 0);
}

// Helper method to broadcast the message to all channel members
void PrivmsgCommand::broadcastMessage(const std::string& message, Channel* channel, RegisteredClient* sender) {
    std::cout << "cucuf333333333\n";
	std::string formattedMessage = ":" + sender->getNickname() + " PRIVMSG " + channel->getName() + " :" + message + "\r\n";
    // Send the message to all channel members except the sender
    std::vector<RegisteredClient*>::const_iterator it;
    for (it = channel->getMembers().begin(); it != channel->getMembers().end(); ++it) {
        RegisteredClient* member = *it;
        if (member != sender) {
            send(member->getFd(), formattedMessage.c_str(), formattedMessage.length(), 0);
        }
    }
}