/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:59:50 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 18:02:30 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAPCOMMAND_HPP
# define CAPCOMMAND_HPP

# include <arpa/inet.h>
# include <netinet/in.h>

# include "ICommand.hpp"

class CapCommand : public ICommand {
	public:
		void executeCommand(int client_fd, const ParsedMessage &msg);
};

#endif