/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:49:33 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 16:08:03 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERCOMMAND_HPP
# define USERCOMMAND_HPP

# include <arpa/inet.h>
# include <netinet/in.h>

# include "ICommand.hpp"

class ClientManager;
class ServerManager;

class UserCommand : public ICommand {
private:
        ClientManager* _clientManager;
        ServerManager* _serverManager;

    public:
        UserCommand(ClientManager* clientManager, ServerManager* serverManager) : 
            _clientManager(clientManager), _serverManager(serverManager) {}
        virtual void executeCommand(int client_fd, const ParsedMessage& msg);
};

#endif