/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelModesCommand.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:41:58 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/21 10:53:48 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// This is a TESTING command

#ifndef CHANNELMODESCOMMAND_HPP
# define CHANNELMODESCOMMAND_HPP

# include <string>
# include <sys/socket.h>

# include "ICommand.hpp"

class ChannelManager;

class ChannelModesCommand : public ICommand {
	private:
		ChannelManager *_channelManager;

	public:
		ChannelModesCommand(ChannelManager *channelManager);
		void executeCommand(int client_fd, const ParsedMessage &parsedMsg);
};

#endif