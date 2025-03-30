/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NoticeCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:25:41 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/27 11:26:19 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NOTICECOMMAND_HPP
# define NOTICECOMMAND_HPP

# include <string>
# include <sys/socket.h>

# include "ICommand.hpp"

class ClientManager;
class ChannelManager;
class Channel;
class RegisteredClient;

class NoticeCommand : public ICommand {
private:
	ChannelManager* _channelManager;
	ClientManager* _clientManager;

public:
	// Constructor and Destructor
	NoticeCommand(ChannelManager* channelManager, ClientManager* clientManager);
	virtual ~NoticeCommand();

	// Methods
	virtual void executeCommand(int client_fd, const ParsedMessage& parsedMsg);
	void sendNoticeToUser(const std::string& message, RegisteredClient* recipient, RegisteredClient* sender);
	void broadcastNotice(const std::string& message, Channel* channel, RegisteredClient* sender);
};

#endif