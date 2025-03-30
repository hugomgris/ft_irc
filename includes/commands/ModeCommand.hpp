/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:41:08 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/21 10:10:59 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define MODECOMMAND_HPP

#include "ICommand.hpp"
#include "../ChannelManager.hpp"
#include "../ClientManager.hpp"

class ModeCommand : public ICommand {
private:
	ChannelManager* _channelManager;
	ClientManager* _clientManager;

public:
	ModeCommand(ChannelManager* channelManager, ClientManager* clientManager);
	~ModeCommand();

	void executeCommand(int client_fd, const ParsedMessage& parsedMsg);
	void broadcast(Channel* channel, const std::string& message, int sender_fd);
};