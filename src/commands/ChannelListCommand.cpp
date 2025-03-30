/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelListCommand.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:09:10 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 17:51:24 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/ChannelListCommand.hpp"
#include "../../includes/ChannelManager.hpp"

//Constructor
ChannelListCommand::ChannelListCommand(ChannelManager *ChannelManager) : _channelManager(ChannelManager) {}

//Method
void ChannelListCommand::executeCommand(int client_fd, const ParsedMessage &parsedMsg) {
	(void)parsedMsg;

	std::string response = "Channels currently available:\r\n";
	send(client_fd, response.c_str(), response.length(), 0);
	_channelManager->printChannelList(client_fd);

	std::cout << "ChannelList command executed by client " << client_fd << std::endl;
}