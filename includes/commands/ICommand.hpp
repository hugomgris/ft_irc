/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:07:12 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/18 16:20:02 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICOMMAND_HPP
# define ICOMMAND_HPP

# include <vector>
# include <iostream>
# include "ParsedMessage.hpp"

class ICommand {
	public:
		virtual ~ICommand() {}
		virtual void executeCommand(int client_fd, const ParsedMessage& parsedMsg) = 0;
};

#endif