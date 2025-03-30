/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:24:49 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/27 11:27:07 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDMANAGER_HPP
# define COMMANDMANAGER_HPP

# include <sstream>
# include <iostream>
# include <map>
# include <vector>
# include <sstream>
# include <algorithm>
# include <cctype>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>

# include "commands/PassCommand.hpp"
# include "commands/NickCommand.hpp"
# include "commands/UserCommand.hpp"
# include "commands/CapCommand.hpp"
# include "commands/PopulationCommand.hpp"
# include "commands/ChannelListCommand.hpp"
# include "commands/ChannelModesCommand.hpp"
# include "commands/QuitCommand.hpp"
# include "commands/JoinCommand.hpp"
# include "commands/PingCommand.hpp"
# include "commands/PongCommand.hpp"
# include "commands/PrivmsgCommand.hpp"
# include "commands/NoticeCommand.hpp"
# include "commands/WhoCommand.hpp"
# include "commands/ModeCommand.hpp"
# include "commands/TopicCommand.hpp"
# include "commands/InviteCommand.hpp"
# include "commands/KickCommand.hpp"


class ServerManager;
class ChannelManager;

class CommandManager {
	private:
		std::map<std::string, ICommand*> _commands;

	public:
		// Constructor and Destructor
		CommandManager(ClientManager* clientManager, ChannelManager *channelmanager, ServerManager* serverManager);
		~CommandManager();

		// Methods
		void executeCommand(int client_fd, const std::string &input);
		ParsedMessage parseInput(const std::string &input);
};

#endif