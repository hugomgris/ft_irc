/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:12:15 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/20 09:15:03 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PINGCOMMAND_HPP
# define PINGCOMMAND_HPP

# include <arpa/inet.h>
# include <netinet/in.h>

# include "ICommand.hpp"

class PingCommand : public ICommand {
	public:
		void executeCommand(int client_fd, const ParsedMessage& msg);
};

#endif