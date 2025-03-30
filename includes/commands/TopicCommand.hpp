/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:43:14 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/21 14:11:23 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPICCOMMAND_HPP
# define TOPICCOMMAND_HPP

# include <iostream>

# include "../../includes/objects/Channel.hpp"
# include "../../includes/objects/RegisteredClient.hpp"
# include "ICommand.hpp"

class ChannelManager;
class ClientManager;

class TopicCommand : public ICommand {
	private:
		ChannelManager *_channelManager;
		ClientManager *_clientManager;

	public:
    	TopicCommand(ChannelManager *channelManager, ClientManager *clientManager);
		void executeCommand(int client_fd, const ParsedMessage &parsedMsg);
		void broadcast(Channel* channel, const std::string& message);
};

#endif