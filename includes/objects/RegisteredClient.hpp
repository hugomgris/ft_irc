/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RegisteredClient.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:15:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/26 14:00:56 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REGISTEREDCLIENT_HPP
# define REGISTEREDCLIENT_HPP

# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>

class RegisteredClient {
	private:
		std::string _buffer;
		int _fd;
		std::string _nickname;
		std::string _username;
		std::string _realName;
		bool _online;
		bool _isBot;
		time_t _lastPingTime;
		time_t _lastPongTime;

	public:
		RegisteredClient();
		RegisteredClient(int fd, const std::string &nickname, const std::string &username);
		~RegisteredClient();

		// Getters and Setters
		int getFd() const;
		const std::string &getNickname() const;
		const std::string &getUsername() const;
		const std::string &getRealName() const;
		bool isOnline() const;
		bool isBot() const;
		void setLastPingTime(time_t time);
		void setLastPongTime(time_t time);

		void setFd(int fd);
		void setNickname(const std::string &nickname);
		void setUsername(const std::string &username);
		void setOnline(bool status);
		void setBot(bool status);
		time_t getLastPingTime() const;
		time_t getLastPongTime() const;
		std::string &getBuffer();
		
		//Methods
		std::string getHost() const;
		void appendToBuffer(const std::string& data);
    	
};

#endif
