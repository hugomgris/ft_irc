/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:41:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/21 12:49:44 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/ModeCommand.hpp"

ModeCommand::ModeCommand(ChannelManager* channelManager, ClientManager* clientManager)
	: _channelManager(channelManager), _clientManager(clientManager) {}

ModeCommand::~ModeCommand() {}

void ModeCommand::executeCommand(int client_fd, const ParsedMessage& parsedMsg) {
	if (parsedMsg.params.empty()) {
		std::string errorMsg = ":server 461 MODE :Not enough parameters\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	std::string channelName = parsedMsg.params[0];
	Channel* channel = _channelManager->getChannel(channelName);

	if (!channel) {
		std::string errorMsg = ":server 403 " + channelName + " :No such channel\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	RegisteredClient* client = _clientManager->getClientFromFd(client_fd);
	if (!client || !channel->isOperator(client)) {
		std::string errorMsg = ":server 482 " + channelName + " :You're not a channel operator\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	if (parsedMsg.params.size() < 2) {
		std::string modeReply = ":server 324 " + client->getNickname() + " " + channelName + " " + channel->getModeString() + "\r\n";
		send(client_fd, modeReply.c_str(), modeReply.length(), 0);
		return;
	}

	std::string modes = parsedMsg.params[1];
	std::string argument = (parsedMsg.params.size() > 2) ? parsedMsg.params[2] : "";

	std::string appliedModes = "";
	std::string appliedArguments = "";
    
	bool addMode = true;
	for (size_t i = 0; i < modes.size(); ++i) {
		char mode = modes[i];

		if (mode == '+') {
			addMode = true;
			appliedModes += "+";
			continue;
		} else if (mode == '-') {
			addMode = false;
			appliedModes += "-";
			continue;
		}

		bool modeApplied = false;
		switch (mode) {
			case 'k':
				if (addMode) {
					channel->setPassword(argument);
					modeApplied = true;
					appliedArguments += " " + argument;
				} else {
					channel->clearPassword();
					modeApplied = true;
				}
				break;
			case 'l':
				if (addMode) {
					int limit = std::atoi(argument.c_str());
					channel->setUserLimit(limit);
					modeApplied = true;
					appliedArguments += " " + argument;
				} else {
					channel->clearUserLimit();
					modeApplied = true;
				}
				break;
			case 'i':
				if (addMode) {
					channel->setInviteOnly();
					modeApplied = true;
				} else {
					channel->clearInviteOnly();
					modeApplied = true;
				}
				break;
			case 'o':
				if (addMode) {
					RegisteredClient* operatorClient = _clientManager->getClientFromNickname(argument);
					if (operatorClient) {
						channel->addOperator(operatorClient);
						modeApplied = true;
						appliedArguments += " " + argument;
					} else {
						std::string errorMsg = ":server 401 " + argument + " :No such nick\r\n";
						send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
					}
				} else {
					RegisteredClient* operatorClient = _clientManager->getClientFromNickname(argument);
					if (operatorClient) {
						channel->removeOperator(operatorClient);
						modeApplied = true;
						appliedArguments += " " + argument;
					} else {
						std::string errorMsg = ":server 401 " + argument + " :No such nick\r\n";
						send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
					}
				}
				break;
			case 't':
				if (addMode) {
					channel->setTopicRestriction();
					modeApplied = true;
				} else {
					channel->clearTopicRestriction();
					modeApplied = true;
				}
				break;
			default:
				std::string errorMsg = ":server 501 " + channelName + " :Unknown mode flag\r\n";
				send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
				return;
		}
		
		if (modeApplied) {
			appliedModes += mode;
		}
	}

	if (appliedModes.length() > 0) {
		std::string modeChangeMsg = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHost() + 
								" MODE " + channelName + " " + appliedModes + appliedArguments + "\r\n";
		broadcast(channel, modeChangeMsg, client_fd);
	}
}

void ModeCommand::broadcast(Channel* channel, const std::string& message, int sender_fd) {
    (void)sender_fd;
	for (size_t i = 0; i < channel->getMembers().size(); ++i) {
        RegisteredClient* member = channel->getMembers()[i];
        send(member->getFd(), message.c_str(), message.length(), 0);
    }
}