/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:48:18 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/20 10:46:40 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICKCOMMAND_HPP
# define NICKCOMMAND_HPP

# include <arpa/inet.h>
# include <netinet/in.h>

# include "ICommand.hpp"

class ClientManager;
class ChannelManager;
class Channel;

class NickCommand : public ICommand {
private:
    ClientManager* _clientManager;
    ChannelManager * _channelManager;

public:
    NickCommand(ClientManager* clientManager, ChannelManager *channelManager) : _clientManager(clientManager), _channelManager(channelManager) {}
    virtual void executeCommand(int client_fd, const ParsedMessage& msg);
};

#endif