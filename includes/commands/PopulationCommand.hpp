/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PopulationCommand.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:16:21 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 11:08:55 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//This is a TESTING command

#ifndef POPULATIONCOMMAND_HPP
# define POPULATIONCOMMAND_HPP

# include <string>
# include <sys/socket.h>


# include "ICommand.hpp"

class ClientManager;

class PopulationCommand : public ICommand {
private:
    ClientManager *_clientManager;

public:
    PopulationCommand(ClientManager *clientManager);
    void executeCommand(int client_fd, const ParsedMessage &parsedMsg);
};

#endif