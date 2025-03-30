/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:33:55 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/24 10:15:24 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/JoinCommand.hpp"
#include "../../includes/ChannelManager.hpp"
#include "../../includes/ClientManager.hpp"
#include "../../includes/ServerManager.hpp"

// Message macros
const std::string JoinCommand::CREATIONSCCS = "Channel created with name ";
const std::string JoinCommand::JOINEDMEMBERSCCS = "Joined as member to channel ";
const std::string JoinCommand::JOINEDOPERATORSCCS = "Joined as operator to channel ";
const std::string JoinCommand::CREATIONERR = "Failed to create channel ";
const std::string JoinCommand::JOINEDOPERATORERR = "Failed to join as operator to channel ";
const std::string JoinCommand::JOINEDMEMBERERR = "Failed to join as member to channel ";

// Constructors and destructor
JoinCommand::JoinCommand() {}

JoinCommand::JoinCommand(ChannelManager *channelManager, ClientManager *clientManager, ServerManager *serverManager)
	: _channelManager(channelManager), _clientManager(clientManager), _serverManager(serverManager) {}

JoinCommand::~JoinCommand() {}

//Methods
void JoinCommand::executeCommand(int client_fd, const ParsedMessage &parsedMsg) {
	if (parsedMsg.params.empty()) {
		std::string errorMsg = ":server 461 JOIN :Not enough parameters\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	std::string name = parsedMsg.params[0];
	std::string password = (parsedMsg.params.size() > 1) ? parsedMsg.params[1] : "";

	if (name[0] != '#') {
		name = "#" + name;
	}

	RegisteredClient *client = _clientManager->getClientFromFd(client_fd);
	if (!client) {
		return;
	}

	bool isNewChannel = (_channelManager->getChannelList().find(name) == _channelManager->getChannelList().end());

	if (isNewChannel) {
		if (!_channelManager->createChannel(name)) {
			std::string creationBadOutput = ":server 482 " + name + " :Failed to create channel\r\n";
			send(client_fd, creationBadOutput.c_str(), creationBadOutput.length(), 0);
			return;
		}
	}

	Channel *channel = _channelManager->getChannel(name);
	if (!channel) {
		return;
	}

	std::cout << "Join attempt by: " << client->getNickname() 
		<< " for channel: " << name 
		<< " - isInviteOnly: " << (channel->isInviteOnly() ? "true" : "false")
		<< " - isInvited: " << (channel->isInvited(client) ? "true" : "false") 
		<< std::endl;

	if (channel->isInviteOnly() && !channel->isInvited(client)) {
		std::string errorMsg = ":server 473 " + client->getNickname() + " " + name + " :Cannot join channel (+i) - Invitation required\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	if (!channel->canJoin(client, password)) {
		std::string errorMsg;
		
		if (channel->hasMode('k')) {
			errorMsg = ":server 475 " + client->getNickname() + " " + name + " :Cannot join channel (+k)\r\n";
		}
		else if (channel->hasMode('l') && channel->isFull()) {
			errorMsg = ":server 471 " + client->getNickname() + " " + name + " :Cannot join channel (+l)\r\n";
		}
		else {
			errorMsg = ":server 474 " + client->getNickname() + " " + name + " :Cannot join channel\r\n";
		}
		
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	if (channel->getOperators().empty()) {
		channel->addOperator(client);
	}

	channel->addMember(client);

	channel->removeInvitation(client);

	std::string joinReply = ":" + client->getNickname() + " JOIN " + name + "\r\n";
	send(client_fd, joinReply.c_str(), joinReply.length(), 0);

	std::string nameReply = ":server 353 " + client->getNickname() + " = " + name + " :";
	for (size_t i = 0; i < channel->getOperators().size(); i++) {
		nameReply += "@" + channel->getOperators()[i]->getNickname() + " ";
	}
	for (size_t i = 0; i < channel->getMembers().size(); i++) {
		nameReply += channel->getMembers()[i]->getNickname() + " ";
	}
	nameReply += "\r\n";
	send(client_fd, nameReply.c_str(), nameReply.length(), 0);

	std::string endOfNames = ":server 366 " + client->getNickname() + " " + name + " :End of /NAMES list.\r\n";
	send(client_fd, endOfNames.c_str(), endOfNames.length(), 0);

	broadcast(channel, client_fd);
}

void JoinCommand::broadcast(Channel *channel, int client_fd) {
	RegisteredClient* newClient = _clientManager->getClientFromFd(client_fd);
	std::string joinMessage = ":" + newClient->getNickname() + " JOIN " + channel->getName() + "\r\n";

	for (size_t i = 0; i < channel->getMembers().size(); ++i) {
		RegisteredClient* member = channel->getMembers()[i];
		if (member->getFd() != client_fd) {
			send(member->getFd(), joinMessage.c_str(), joinMessage.length(), 0);
		}
	}
}
