#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include <algorithm>
#include <array>
#include <memory>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <netinet/tcp.h>
#include <sys/types.h>
#include <netdb.h>

#define SOCK_BUFF_SIZE 1024
#define IRC_SERVER "127.0.0.1"
#define IRC_SPORT 4242
#define IRC_SPASS "1qazXSW@"
#define IRC_SNAME "IRC_SERVER"
#define IRC_USER "BotWeather"
#define IRC_HOST "127.0.0.1"
#define IRC_RNAME "Weather"
#define IRC_NICK "ircbot"

class Socket{
	private:
		int				fd_;
		std::string		host_;
		in_port_t		port_;
		sockaddr_in		addr_;
		void set();
	
	public:
		Socket();
		~Socket()						{ close(fd_); }
		int getFd()						{ return fd_;}
		void tryToConnect();
		int tryToSend(const std::string& msg);
		std::string tryToRecv();
};

#endif