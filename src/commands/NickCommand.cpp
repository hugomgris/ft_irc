/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:52:35 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 17:17:58 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/NickCommand.hpp"
#include "../../includes/ClientManager.hpp"
#include "../../includes/ChannelManager.hpp"
#include "../../includes/objects/Channel.hpp"

void NickCommand::executeCommand(int client_fd, const ParsedMessage& msg) {
	if (msg.params.empty()) {
		send(client_fd, "431 :No nickname given\r\n", 24, 0);
		return;
	}

	std::string newNickname = msg.params[0];
	RegisteredClient* client = _clientManager->getClientFromFd(client_fd);

	if (_clientManager->isNicknameRegistered(newNickname)) {
		std::string response = "433 " + (client ? client->getNickname() : "*") + " " + newNickname + " :Nickname is already in use\r\n";
		send(client_fd, response.c_str(), response.length(), 0);
		return;
	}

	if (!client) {
		_clientManager->addPendingNickname(client_fd, newNickname);
		if (_clientManager->isPendingRegistrationComplete(client_fd)) {
			_clientManager->completePendingRegistration(client_fd);
		}
		return;
	}

	std::string oldNickname = client->getNickname();
	client->setNickname(newNickname);
	
	std::string nickChangeMessage = ":" + oldNickname + " NICK :" + newNickname + "\r\n";

	std::vector<Channel*> channels = _channelManager->getClientChannels(client_fd);
	for (size_t i = 0; i < channels.size(); i++) {
		channels[i]->broadcastMessage(nickChangeMessage, client);
	}

	send(client_fd, nickChangeMessage.c_str(), nickChangeMessage.length(), 0);
}