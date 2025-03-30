/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:20:39 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/24 10:52:14 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/InviteCommand.hpp"
#include "../../includes/ChannelManager.hpp"
#include "../../includes/ClientManager.hpp"
#include "../../includes/objects/Channel.hpp"

// Constructor
InviteCommand::InviteCommand(ChannelManager *channelManager, ClientManager *clientManager)
	: _channelManager(channelManager), _clientManager(clientManager) {}

// Method
void InviteCommand::executeCommand(int client_fd, const ParsedMessage &parsedMsg) {
	RegisteredClient* client = _clientManager->getClientFromFd(client_fd);
	if (!client) {
		std::string errorMsg = ":server 400 :Unknown error\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	if (parsedMsg.params.size() < 2) {
		std::string errorMsg = ":server 461 " + client->getNickname() + " INVITE :Not enough parameters\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	std::string inviteeNick = parsedMsg.params[0];
	std::string channelName = parsedMsg.params[1];

	Channel* channel = _channelManager->getChannel(channelName);
	if (!channel) {
		std::string errorMsg = ":server 403 " + client->getNickname() + " " + channelName + " :No such channel\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	if (!channel->isMember(client)) {
		std::string errorMsg = ":server 442 " + client->getNickname() + " " + channelName + " :You're not on that channel\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	if (!channel->isOperator(client)) {
		std::string errorMsg = ":server 482 " + client->getNickname() + " " + channelName + " :You're not a channel operator\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	RegisteredClient* invitee = _clientManager->getClientFromNickname(inviteeNick);
	if (!invitee) {
		std::string errorMsg = ":server 401 " + client->getNickname() + " " + inviteeNick + " :No such nick\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	if (channel->isMember(invitee)) {
		std::string errorMsg = ":server 443 " + client->getNickname() + " " + inviteeNick + " " + channelName + " :Is already on channel\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	channel->inviteClient(invitee);
	std::cout << "Inviting user: " << inviteeNick << " to channel: " << channelName << std::endl;

	std::string inviteSuccessMsg = ":server 341 " + client->getNickname() + " " + inviteeNick + " " + channelName + "\r\n";
	send(client_fd, inviteSuccessMsg.c_str(), inviteSuccessMsg.length(), 0);

	std::string inviteMsg = ":" + client->getNickname() + " INVITE " + inviteeNick + " " + channelName + "\r\n";
	send(invitee->getFd(), inviteMsg.c_str(), inviteMsg.length(), 0);
	
	std::string broadcastMsg = ":" + client->getNickname() + " NOTICE " + channelName + " :" + inviteeNick + " was invited to the channel\r\n";
	broadcast(channel, broadcastMsg, client_fd);
}

void InviteCommand::broadcast(Channel* channel, const std::string& message, int sender_fd) {
	(void)sender_fd;

	for (size_t i = 0; i < channel->getOperators().size(); ++i) {
		RegisteredClient* oper = channel->getOperators()[i];
		send(oper->getFd(), message.c_str(), message.length(), 0);
	}
}