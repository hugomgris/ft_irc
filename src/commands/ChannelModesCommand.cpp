/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelModesCommand.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:46:52 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/21 12:32:19 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// This is a TESTING command

#include "../../includes/commands/ChannelModesCommand.hpp"
#include "../../includes/ChannelManager.hpp"

ChannelModesCommand::ChannelModesCommand(ChannelManager *channelManager): _channelManager(channelManager) {}

void ChannelModesCommand::executeCommand(int client_fd, const ParsedMessage &parsedMsg) {
	if (parsedMsg.params.empty()) {
		std::string noParamsOutput = "Please indicate channel #name\r\n";
		send(client_fd, noParamsOutput.c_str(), noParamsOutput.length(), 0);
		return ;
	}

	std::cout << "param:" << parsedMsg.params[0] << std::endl;

	std::map<std::string, Channel> channelList = _channelManager->getChannelList();
	if (channelList.empty()) {
		std::string noChannelsOutput = "No channels available\r\n";
		send(client_fd, noChannelsOutput.c_str(), noChannelsOutput.length(), 0);
		return ;
	}

	Channel *channel = _channelManager->getChannel(parsedMsg.params[0]);
	
	if (!channel) {
		std::string badChannelOutput = "No channel found with name #" + parsedMsg.params[0] + "\r\n";
		send(client_fd, badChannelOutput.c_str(), badChannelOutput.length(), 0);
		return ;
	}

	std::string channelName = channel->getName();

	std::string modeOutput = "Channel " + channelName + " has modes: ";
	bool firstMode = true;

	if (channel->hasMode('k')) {
		if (!firstMode) modeOutput += ", ";
		modeOutput += "+k";
		firstMode = false;
	} else {
		if (!firstMode) modeOutput += ", ";
		modeOutput += "-k";
		firstMode = false;
	}

	if (channel->hasMode('l')) {
		if (!firstMode) modeOutput += ", ";
		modeOutput += "+l";
		firstMode = false;
	} else {
		if (!firstMode) modeOutput += ", ";
		modeOutput += "-l";
		firstMode = false;
	}

	if (channel->hasMode('i')) {
		if (!firstMode) modeOutput += ", ";
		modeOutput += "+i";
		firstMode = false;
	} else {
		if (!firstMode) modeOutput += ", ";
		modeOutput += "-i";
		firstMode = false;
	}

	if (channel->hasMode('o')) {
		if (!firstMode) modeOutput += ", ";
		modeOutput += "+o";
		firstMode = false;
	} else {
		if (!firstMode) modeOutput += ", ";
		modeOutput += "-o";
		firstMode = false;
	}

	if (channel->hasMode('t')) {
		if (!firstMode) modeOutput += ", ";
		modeOutput += "+t";
		firstMode = false;
	} else {
		if (!firstMode) modeOutput += ", ";
		modeOutput += "-t";
		firstMode = false;
	}

	modeOutput += "\r\n";
	send(client_fd, modeOutput.c_str(), modeOutput.length(), 0);
}