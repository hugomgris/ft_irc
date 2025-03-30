/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 16:06:15 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 18:16:01 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/UserCommand.hpp"
#include "../../includes/ClientManager.hpp"

void UserCommand::executeCommand(int client_fd, const ParsedMessage& msg) {
    if (msg.params.size() < 4) {
        send(client_fd, "461 USER :Not enough parameters\r\n", 33, 0);
        return;
    }
    
    std::string username = msg.params[0];
    _clientManager->addPendingUsername(client_fd, username);
    
    if (_clientManager->isPendingRegistrationComplete(client_fd)) {
        _clientManager->completePendingRegistration(client_fd);
        
        // Send welcome messages
        std::string nickname = _clientManager->getPendingNickname(client_fd);
        if (!nickname.empty()) {
            std::string welcome = "001 " + nickname + " :Welcome to the IRC network\r\n";
            send(client_fd, welcome.c_str(), welcome.length(), 0);
            
            // We can add more welcome messages here (002, 003, 004, etc.)
            std::string yourHost = "002 " + nickname + " :Your host is ft_irc, running version 1.0\r\n";
            send(client_fd, yourHost.c_str(), yourHost.length(), 0);
            
            std::string created = "003 " + nickname + " :This server was created today\r\n";
            send(client_fd, created.c_str(), created.length(), 0);
            
            std::string myInfo = "004 " + nickname + " ft_irc 1.0 o o\r\n";
            send(client_fd, myInfo.c_str(), myInfo.length(), 0);
        }
    }
}