/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:37:59 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/24 10:51:44 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICKCOMMAND_HPP
#define KICKCOMMAND_HPP

#include "ICommand.hpp"
#include <string>

class ChannelManager;
class ClientManager;
class Channel;

class KickCommand : public ICommand {
private:
    ChannelManager *_channelManager;
    ClientManager *_clientManager;

public:
    // Constructors & Destructor
    KickCommand();
    KickCommand(ChannelManager *channelManager, ClientManager *clientManager);
    ~KickCommand();

    // Method
    void executeCommand(int client_fd, const ParsedMessage &parsedMsg);
	void broadcast(Channel* channel, const std::string& message, int sender_fd);
};

#endif