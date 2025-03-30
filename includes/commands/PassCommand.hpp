/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:47:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 15:52:15 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASSCOMMAND_HPP
# define PASSCOMMAND_HPP

# include <arpa/inet.h>
# include <netinet/in.h>

# include "ICommand.hpp"

class ClientManager;

class PassCommand : public ICommand {
	private:
		ClientManager* _clientManager;

	public:
		PassCommand(ClientManager* clientManager) : _clientManager(clientManager) {}
		virtual void executeCommand(int client_fd, const ParsedMessage& msg);
};

#endif