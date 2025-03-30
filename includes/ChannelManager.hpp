/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:29:13 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/26 16:31:34 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELMANAGER_HPP
# define CHANNELMANAGER_HPP

# include <map>
# include <vector>
# include <string>
# include <sstream> 
# include "objects/Channel.hpp"
# include <sys/types.h>
# include <sys/socket.h>

class ClientManager;
class RegisteredClient;

class ChannelManager {
	private:
    	std::map<std::string, Channel> _channels;
		ClientManager *_clientManager;

		RegisteredClient *_bot;

	public:
		// Constructor and Destructor
		ChannelManager(ClientManager *clientManager);
		~ChannelManager();

		// Getters and Setters
		const std::map<std::string, Channel> &getChannelList() const;
		Channel* getChannel(const std::string &name);
		std::vector<Channel*> getClientChannels(int client_fd);

		// Methods
		bool createChannel(const std::string &name);
		bool removeChannel(const std::string &name);
		void removeClientFromAllChannels(RegisteredClient *client);
		void removeClientFromAllInviteLists(RegisteredClient* client);


		// Testing method
		void printChannelList(int client_fd);
};

#endif