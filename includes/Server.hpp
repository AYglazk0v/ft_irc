#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
# include <sys/select.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <vector>

# include "Ping.hpp"
# include "User.hpp"
# include "Chanel.hpp"

#define	MAX_CLIENT	1000
#define	MAX_CHANEL	1000

class Server {
	private:
		int						port_;
		int						socket_;
		int						user_connectin_;
		std::string				password_;
		fd_set					read_;
		fd_set					write_;
		fd_set					accept_;
		timeval					time_;
		Ping					req_res[MAX_CLIENT];
		struct sockaddr_in		addr_;
		std::vector<User *>		users_;
		std::vector<User *> 	history_;
		std::vector<Chanel *>	chanels;

	public:
		Server();
		Server(char **argv);
		~Server();

}; //server

#endif