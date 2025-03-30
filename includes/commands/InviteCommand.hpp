/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:20:01 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/24 10:52:05 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVITECOMMAND_HPP
# define INVITECOMMAND_HPP

# include <iostream>

# include "../../includes/objects/Channel.hpp"
# include "../../includes/objects/RegisteredClient.hpp"
# include "ICommand.hpp"

class ChannelManager;
class ClientManager;
class Channel;

class InviteCommand : public ICommand {
	private:
		ChannelManager *_channelManager;
		ClientManager *_clientManager;

	public:
		InviteCommand(ChannelManager *channelManager, ClientManager *clientManager);
		void executeCommand(int client_fd, const ParsedMessage &parsedMsg);
		void broadcast(Channel* channel, const std::string& message, int sender_fd);
};

#endif