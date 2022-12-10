#ifndef SERVER_HPP
# define SERVER_HPP

# include "utils.hpp"
# include "Ping.hpp"
# include "User.hpp"
# include "Chanel.hpp"
#include <vector>

#define	MAX_CLIENT	1000
#define	MAX_CHANEL	1000

class Server {
	private:
		int						port_;
		int						socket_;
		int						user_connection_;
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

	private:
		void new_user_connect();
		void start_read();
		void disconnect(User*);

	public:
		Server(char **argv);
		void	loop();

}; //server

#endif