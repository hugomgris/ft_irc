/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:15:50 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/17 15:56:45 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/SocketManager.hpp"
#include "../includes/ClientManager.hpp"
#include "../includes/ServerManager.hpp"

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	try {
		ServerManager server(std::atoi(argv[1]), argv[2]);
		server.run();
	} catch (const std::exception &e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}

	return 0;
}