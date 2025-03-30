/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:45:48 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/21 14:25:48 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/TopicCommand.hpp"
#include "../../includes/ChannelManager.hpp"
#include "../../includes/ClientManager.hpp"

//Constructor
TopicCommand::TopicCommand(ChannelManager *channelManager, ClientManager *clientManager): _channelManager(channelManager), _clientManager(clientManager) {}

// Method
void TopicCommand::executeCommand(int client_fd, const ParsedMessage &parsedMsg) {
    RegisteredClient* client = _clientManager->getClientFromFd(client_fd);
    if (!client) {
        std::string errorMsg = ":server 400 :Unknown error\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    if (parsedMsg.params.empty()) {
        std::string errorMsg = ":server 461 " + client->getNickname() + " " + parsedMsg.command + " :Not enough parameters\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    std::string channelName = parsedMsg.params[0];

	if (channelName == "ft_irc") {
	std::string errorMsg = ":server 461 " + client->getNickname() + " " + parsedMsg.command + " :Please specify a channel name (e.g., /TOPIC #channelname)\r\n";
	send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
	return;
	}

    if (channelName[0] != '#' && channelName[0] != '&') {
        std::string errorMsg = ":server 403 " + client->getNickname() + " " + channelName + " :No such channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

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

    if (parsedMsg.params.size() == 1) {
        std::string topicReply;
        if (channel->getTopic().empty()) {
            topicReply = ":server 331 " + client->getNickname() + " " + channelName + " :No topic is set\r\n";
        } else {
            topicReply = ":server 332 " + client->getNickname() + " " + channelName + " :" + channel->getTopic() + "\r\n";
        }
        send(client_fd, topicReply.c_str(), topicReply.length(), 0);
        return;
    }

    if (channel->isTopicRestricted() && !channel->isOperator(client)) {
        std::string errorMsg = ":server 482 " + client->getNickname() + " " + channelName + " :You're not a channel operator\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    std::string newTopic = parsedMsg.params[1];
    channel->setTopic(newTopic);

    std::string topicChangeMsg = ":" + client->getNickname() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
    broadcast(channel, topicChangeMsg);
}

void TopicCommand::broadcast(Channel* channel, const std::string& message) {
    const std::vector<RegisteredClient*>& members = channel->getMembers();
    for (size_t i = 0; i < members.size(); ++i) {
        RegisteredClient* member = members[i];
        send(member->getFd(), message.c_str(), message.length(), 0);
    }
}