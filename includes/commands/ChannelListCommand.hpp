/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelListCommand.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:06:27 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 11:14:42 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//This is a TESTING command

#ifndef CHANNELLISTCOMMAND
# define CHANNELLISTCOMMAND

# include <string>
# include <sys/socket.h>

# include "ICommand.hpp"

class ChannelManager;

class ChannelListCommand : public ICommand {
	private:
		ChannelManager *_channelManager;

	public:
		ChannelListCommand(ChannelManager *channelManager);
		void executeCommand(int client_fd, const ParsedMessage &parsedMsg);
};

#endif