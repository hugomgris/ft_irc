/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NoticeCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:26:38 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/27 11:26:55 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/NoticeCommand.hpp"
#include "../../includes/ClientManager.hpp"
#include "../../includes/ChannelManager.hpp"

// Constructor and Destructor
NoticeCommand::NoticeCommand(ChannelManager* channelManager, ClientManager* clientManager): 
	_channelManager(channelManager), _clientManager(clientManager) {}

NoticeCommand::~NoticeCommand() {}

// Methods
void NoticeCommand::executeCommand(int client_fd, const ParsedMessage& parsedMsg) {
	if (parsedMsg.params.size() < 2) {
		return;
	}

	std::string target = parsedMsg.params[0];
	std::string message = parsedMsg.params[1];

	RegisteredClient* sender = _clientManager->getClientFromFd(client_fd);
	if (!sender) {
		return;
	}

	if (target[0] == '#') {
		Channel* channel = _channelManager->getChannel(target);
		if (!channel || !channel->isMember(sender)) {
			return;
		}

		broadcastNotice(message, channel, sender);
	} else {
		RegisteredClient* recipient = _clientManager->getClientFromNickname(target);
		if (!recipient) {
			return;
		}

		sendNoticeToUser(message, recipient, sender);
	}
}

void NoticeCommand::sendNoticeToUser(const std::string& message, RegisteredClient* recipient, RegisteredClient* sender) {
	std::string formattedMessage = ":" + sender->getNickname() + " NOTICE " + recipient->getNickname() + " :" + message + "\r\n";

	send(recipient->getFd(), formattedMessage.c_str(), formattedMessage.length(), 0);
}

void NoticeCommand::broadcastNotice(const std::string& message, Channel* channel, RegisteredClient* sender) {
	std::string formattedMessage = ":" + sender->getNickname() + " NOTICE " + channel->getName() + " :" + message + "\r\n";

	std::vector<RegisteredClient*>::const_iterator it;
	for (it = channel->getMembers().begin(); it != channel->getMembers().end(); ++it) {
		RegisteredClient* member = *it;
		if (member != sender) {
			send(member->getFd(), formattedMessage.c_str(), formattedMessage.length(), 0);
		}
	}
}