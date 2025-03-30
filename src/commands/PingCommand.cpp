/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:14:24 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/20 09:14:49 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/PingCommand.hpp"

void PingCommand::executeCommand(int client_fd, const ParsedMessage& msg) {
	if (msg.params.empty()) {
		send(client_fd, "409 :No origin specified\r\n", 26, 0);
		return;
	}

	std::string response = "PONG :" + msg.params[0] + "\r\n";
	send(client_fd, response.c_str(), response.length(), 0);
}