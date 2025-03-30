/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:31:42 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/20 11:38:43 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOINCOMMAND_HPP
# define JOINCOMMAND_HPP

# include "ICommand.hpp"

class ChannelManager;
class ClientManager;
class ServerManager;
class Channel;

class JoinCommand : public ICommand {
	private:
		ChannelManager *_channelManager;
		ClientManager *_clientManager;
		ServerManager *_serverManager;

		//Message macros
		static const std::string CREATIONSCCS;
		static const std::string JOINEDMEMBERSCCS;
		static const std::string JOINEDOPERATORSCCS;
		static const std::string CREATIONERR;
		static const std::string JOINEDMEMBERERR;
		static const std::string JOINEDOPERATORERR;

	public:
		//Constructors and Destructor
		JoinCommand();
		~JoinCommand();
		JoinCommand(ChannelManager *channelManager, ClientManager *clientManager, ServerManager *serverManager);

		//Method
		void executeCommand(int client_fd, const ParsedMessage &parsedMsg);
		std::string getMemberList(Channel* channel);
		void broadcast(Channel *channel, int client_fd);
};

#endif