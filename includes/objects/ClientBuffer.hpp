/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientBuffer.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:15:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/25 12:56:38 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTBUFFER_HPP
# define CLIENTBUFFER_HPP

# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>

class ClientBuffer {
	private:
		std::string _buffer;
		int _fd;

	public:
		ClientBuffer();
		ClientBuffer(int client_fd);
		~ClientBuffer();

		// Getters and Setters
		int getFd() const;

		void setFd(int fd);
		std::string &getBuffer();
		
		//Methods
		void appendToBuffer(const std::string& data);
		void clearBuffer();
    	
};

#endif