/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PongCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:33:37 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/20 09:43:55 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/PongCommand.hpp"
#include "../../includes/ClientManager.hpp"
#include <iostream>

void PongCommand::executeCommand(int client_fd, const ParsedMessage& parsedMsg) {
	if (parsedMsg.params.empty()) {
		std::string errorMsg = "461 PONG :Not enough parameters\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	std::string token = parsedMsg.params[0];

	RegisteredClient* client = _clientManager->getClientFromFd(client_fd);
	if (!client) {
		std::cerr << "PONG received from unknown client (fd: " << client_fd << ")\n";
		return;
	}

	client->setLastPongTime(time(NULL));

	std::cout << "PONG received from " << client->getNickname() << " with token: " << token << std::endl;
}