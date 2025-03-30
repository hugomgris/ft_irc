/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 10:40:22 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/20 16:27:48 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSGCOMMAND_HPP
# define PRIVMSGCOMMAND_HPP

# include <string>
# include <sys/socket.h>

# include "ICommand.hpp"

class ClientManager;
class ChannelManager;
class Channel;
class RegisteredClient;

class PrivmsgCommand : public ICommand {
private:
	ChannelManager* _channelManager;
	ClientManager* _clientManager;

public:
	// Constructor and Destructor
	PrivmsgCommand(ChannelManager* channelManager, ClientManager* clientManager);
	virtual ~PrivmsgCommand();

	// Methods
	virtual void executeCommand(int client_fd, const ParsedMessage& parsedMsg);
	void sendMessageToUser(const std::string& message, RegisteredClient* recipient, RegisteredClient* sender);
	void broadcastMessage(const std::string& message, Channel* channel, RegisteredClient* sender);
};

#endif