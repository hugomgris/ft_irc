/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:01:49 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 18:08:51 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/CapCommand.hpp"

void CapCommand::executeCommand(int client_fd, const ParsedMessage &msg) {
	if (msg.params.empty()) {
		send(client_fd, "CAP * LS :\r\n", 12, 0); // No capabilities
	} else if (msg.params[0] == "LS") {
		send(client_fd, "CAP * LS :\r\n", 12, 0); // List capabilities (none)
	} else if (msg.params[0] == "REQ") {
		send(client_fd, ("CAP * NAK :" + msg.params[1] + "\r\n").c_str(), 15 + msg.params[1].length(), 0);
	} else if (msg.params[0] == "END") {
		// No response needed for END
	}
}
