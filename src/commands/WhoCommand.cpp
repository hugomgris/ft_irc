/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:17:34 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/26 16:39:16 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/WhoCommand.hpp"

# include "../../includes/ClientManager.hpp"
# include "../../includes/ServerManager.hpp"
# include "../../includes/ChannelManager.hpp"
# include <cerrno>
# include <cstdio>

WhoCommand::WhoCommand(ServerManager *serverManager, ChannelManager *channelManager)
    : _serverManager(serverManager), _channelManager(channelManager) {}

void WhoCommand::executeCommand(int client_fd, const ParsedMessage& parsedMsg) {
    if (parsedMsg.params.empty()) {
        std::string errorMsg = ":server 461 Who :Not enough parameters\r\n";
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

    sendWhoReply(client_fd, channel);
}

void WhoCommand::sendWhoReply(int client_fd, Channel* channel) {
    RegisteredClient* requester = _serverManager->getClientManager()->getClientFromFd(client_fd);
    if (!requester) {
        std::cerr << "WHO: Requesting client not found!" << std::endl;
        return;
    }

    std::cout << "WHO command received for channel: " << channel->getName() << std::endl;

    for (size_t i = 0; i < channel->getMembers().size(); ++i) {
        RegisteredClient* member = channel->getMembers()[i];

        std::string status = (channel->isOperator(member) ? "@" : "");

        std::string whoMessage = ":" + _serverManager->getServerName() + " 352 " +
                                 requester->getNickname() + " " +
                                 channel->getName() + " " +
                                 member->getUsername() + " " +
                                 member->getHost() + " " + 
                                 _serverManager->getServerName() + " " + 
                                 member->getNickname() + " " +
                                 status + "H :0 " +
                                 member->getRealName() + "\r\n";

        std::cout << "WHO Reply for " << member->getNickname() 
                  << " | Real Name: " << member->getRealName() << std::endl;

        ssize_t sentBytes = send(client_fd, whoMessage.c_str(), whoMessage.length(), 0);
        if (sentBytes == -1) {
            perror("send WHO reply failed");
        }
    }

    std::string endMessage = ":" + _serverManager->getServerName() + " 315 " + 
                             requester->getNickname() + " " + 
                             channel->getName() + " :End of WHO list.\r\n";

    std::cout << "Sending End of WHO List: " << endMessage << std::endl;

    ssize_t sentEnd = send(client_fd, endMessage.c_str(), endMessage.length(), 0);
    if (sentEnd == -1) {
        perror("send End of WHO List failed");
    }
}
