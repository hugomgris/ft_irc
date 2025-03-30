/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:50:39 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 15:51:47 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/PassCommand.hpp"
#include "../../includes/ClientManager.hpp"

void PassCommand::executeCommand(int client_fd, const ParsedMessage& msg) {
    if (msg.params.empty()) {
        send(client_fd, "461 PASS :Not enough parameters\r\n", 33, 0);
        return;
    }
    
    std::string password = msg.params[0];
    _clientManager->addPendingPassword(client_fd, password);
    
    // Check if all registration info is available
    if (_clientManager->isPendingRegistrationComplete(client_fd)) {
        _clientManager->completePendingRegistration(client_fd);
    }
}