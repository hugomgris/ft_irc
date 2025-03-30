/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:38:32 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/25 17:43:04 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/KickCommand.hpp"
#include "../../includes/ChannelManager.hpp"
#include "../../includes/ClientManager.hpp"
#include "../../includes/objects/Channel.hpp"

// Constructor
KickCommand::KickCommand()
    : _channelManager(NULL), _clientManager(NULL) {}

KickCommand::KickCommand(ChannelManager *channelManager, ClientManager *clientManager)
    : _channelManager(channelManager), _clientManager(clientManager) {}

// Destructor
KickCommand::~KickCommand() {}

// Method
void KickCommand::executeCommand(int client_fd, const ParsedMessage &parsedMsg) {
    RegisteredClient* operator_client = _clientManager->getClientFromFd(client_fd);
    if (!operator_client) {
        std::string errorMsg = ":server 400 :Unknown error\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    if (parsedMsg.params.size() < 2) {
        std::string errorMsg = ":server 461 " + operator_client->getNickname() + " KICK :Not enough parameters\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    std::string channelName = parsedMsg.params[0];
    std::string targetNick = parsedMsg.params[1];
    std::string kickReason = (parsedMsg.params.size() > 2) ? parsedMsg.params[2] : operator_client->getNickname();

    Channel* channel = _channelManager->getChannel(channelName);
    if (!channel) {
        std::string errorMsg = ":server 403 " + operator_client->getNickname() + " " + channelName + " :No such channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    if (!channel->isMember(operator_client)) {
        std::string errorMsg = ":server 442 " + operator_client->getNickname() + " " + channelName + " :You're not on that channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    if (!channel->isOperator(operator_client)) {
        std::string errorMsg = ":server 482 " + operator_client->getNickname() + " " + channelName + " :You're not a channel operator\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    RegisteredClient* targetClient = _clientManager->getClientFromNickname(targetNick);
    if (!targetClient) {
        std::string errorMsg = ":server 401 " + operator_client->getNickname() + " " + targetNick + " :No such nick\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    //! delete before submission
    std::cout << "Debug - Checking if " << targetNick << " is member of " << channelName << std::endl;
    std::cout << "Channel members:" << std::endl;
    for (size_t i = 0; i < channel->getMembers().size(); i++) {
        std::cout << " - " << channel->getMembers()[i]->getNickname() << std::endl;
    }
    std::cout << "Channel operators:" << std::endl;
    for (size_t i = 0; i < channel->getOperators().size(); i++) {
        std::cout << " - " << channel->getOperators()[i]->getNickname() << std::endl;
    }

    if (!channel->isMember(targetClient)) {
        std::string errorMsg = ":server 441 " + operator_client->getNickname() + " " + targetNick + " " + channelName + " :They aren't on that channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    std::string kickMsg = ":" + operator_client->getNickname() + " KICK " + channelName + " " + targetNick + " :" + kickReason + "\r\n";
    broadcast(channel, kickMsg, client_fd);

    if (channel->isOperator(targetClient)) {
        channel->removeOperator(targetClient);
    }
    channel->removeMember(targetClient);
	_channelManager->removeClientFromAllInviteLists(targetClient);

    std::cout << "User " << targetNick << " kicked from channel " << channelName << " by " << operator_client->getNickname() << std::endl;
}

void KickCommand::broadcast(Channel* channel, const std::string& message, int sender_fd) {
    (void)sender_fd;	
    
    for (size_t i = 0; i < channel->getMembers().size(); ++i) {
        RegisteredClient* member = channel->getMembers()[i];
        send(member->getFd(), message.c_str(), message.length(), 0);
    }
}