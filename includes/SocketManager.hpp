/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:00:16 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/14 16:19:25 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETMANAGER_HPP
# define SOCKETMANAGER_HPP

# define BUFFER_SIZE 1024

# include <iostream>
# include <cstring>
# include <cstdlib>
# include <sys/socket.h> // For socket API functions (socket, bind, listen, etc.)
# include <netinet/in.h> // For internet address structures (sockaddr_in)
# include <unistd.h>

class SocketManager{
	private:
		//Coplien stuff
		SocketManager();
		SocketManager(const SocketManager &other);
		SocketManager &operator=(const SocketManager &other);

		int _port;
		int _server_fd;
		int _client_fd;
		struct sockaddr_in _server_addr;

	public:
		//Exception classes
		class SocketFailureException : public std::exception{
			const char *what() const throw();
		};
		
		//Constructor and Destructor
		SocketManager(int port);
		~SocketManager();

		//Getters and Setters
		int getServerFd() const;
		int getClientFd() const;
		void setClientFd(int &client_fd);

		//Methods
		bool createSocket();
		bool bindSocket();
		bool listenForConnections();
		int acceptConnection();
		void closeSocket(int client_fd);

		void pollForClients();
};

#endif