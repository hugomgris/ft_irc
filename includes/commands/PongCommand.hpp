/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PongCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:34:19 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/20 09:43:38 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PONGCOMMAND_HPP
#define PONGCOMMAND_HPP

#include "../commands/ICommand.hpp"

class ClientManager;

class PongCommand : public ICommand {
	private:
		ClientManager *_clientManager;
	
	public:
		PongCommand(ClientManager *clientManager): _clientManager(clientManager) {}
		void executeCommand(int client_fd, const ParsedMessage& parsedMsg);
};

#endif