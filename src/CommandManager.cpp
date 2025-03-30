/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:24:23 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/27 12:33:26 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CommandManager.hpp"
#include "../includes/ChannelManager.hpp"

// Constructor and Destructor

CommandManager::CommandManager(ClientManager *clientManager, ChannelManager *channelManager, ServerManager *serverManager) {
	_commands["PASS"] = new PassCommand(clientManager);
	_commands["NICK"] = new NickCommand(clientManager, channelManager);
	_commands["USER"] = new UserCommand(clientManager, serverManager);
	_commands["CAP"] = new CapCommand();
	_commands["POPULATION"] = new PopulationCommand(clientManager);
	_commands["CHANNELLIST"] = new ChannelListCommand(channelManager);
	_commands["CHANNELMODES"] = new ChannelModesCommand(channelManager);
	_commands["QUIT"] = new QuitCommand(serverManager, channelManager);
	_commands["PING"] = new PingCommand();
	_commands["PONG"] = new PongCommand(clientManager);
	_commands["JOIN"] = new JoinCommand(channelManager, clientManager, serverManager);
	_commands["PRIVMSG"] = new PrivmsgCommand(channelManager, clientManager);
	_commands["NOTICE"] = new NoticeCommand(channelManager, clientManager);
	_commands["MODE"] = new ModeCommand(channelManager, clientManager);
	_commands["TOPIC"] = new TopicCommand(channelManager, clientManager);
	_commands["WHO"] = new WhoCommand(serverManager, channelManager);
	_commands["INVITE"] = new InviteCommand(channelManager, clientManager);
	_commands["KICK"] = new KickCommand(channelManager, clientManager);
}

CommandManager::~CommandManager() {
	for (std::map<std::string, ICommand*>::iterator it = _commands.begin(); it != _commands.end(); ++it) {
		delete it->second;
	}
}

ParsedMessage CommandManager::parseInput(const std::string &input){
	ParsedMessage parsedMsg;
	std::istringstream stream(input);
	std::string token;

	if (!input.empty() && input[0] == ':') {
		stream >> parsedMsg.prefix;
		parsedMsg.prefix = parsedMsg.prefix.substr(1);
	}

	if (stream >> parsedMsg.command) {
		while (stream >> token) {
			if (token[0] == ':') {
				std::string rest;
				std::getline(stream, rest);
				parsedMsg.params.push_back(token.substr(1) + rest);
				break;
			}
			parsedMsg.params.push_back(token);
		}
	}

	return parsedMsg;
}

void CommandManager::executeCommand(int client_fd, const std::string &input) {
	std::istringstream stream(input);
	std::string line;

	while (std::getline(stream, line)) {
		if (!line.empty() && line[line.size() - 1] == '\r') {
			line.erase(line.size() - 1);
		}

		ParsedMessage parsedMsg = parseInput(line);

		if (parsedMsg.command.empty()) {
			continue;
		}

		std::transform(parsedMsg.command.begin(), parsedMsg.command.end(), 
					   parsedMsg.command.begin(), ::toupper);

		if (_commands.find(parsedMsg.command) != _commands.end()) {
			std::cout << parsedMsg.command << std::endl;
			_commands[parsedMsg.command]->executeCommand(client_fd, parsedMsg);

			std::cout << "Client with fd " << client_fd << " executed command " << parsedMsg.command << std::endl;
		} else {
			std::string commandError = "Unknown command: " + parsedMsg.command + "\r\n";
			send(client_fd, commandError.c_str(), commandError.length(), 0);
		}
	}
}
