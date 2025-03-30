/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:17:01 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/20 12:33:14 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WHOCOMMAND_HPP
# define WHOCOMMAND_HPP

# include <sstream>

# include "ICommand.hpp"

class ServerManager;
class Clientmanager;
class ChannelManager;
class Channel;

class WhoCommand : public ICommand {
private:
	ServerManager *_serverManager;
	ChannelManager *_channelManager;

	void sendWhoReply(int client_fd, Channel* channel);

public:
	WhoCommand(ServerManager *serverManager, ChannelManager * channelManager);
	~WhoCommand() {}

	void executeCommand(int client_fd, const ParsedMessage& parsedMsg);
};

#endif