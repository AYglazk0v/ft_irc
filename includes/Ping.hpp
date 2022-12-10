#ifndef PING_HPP
# define PING_HPP

# include "utils.hpp"
# include <ctime>

struct Ping {
		int				client_sock_fd;
		bool			disconnect;
		bool			res_wait;
		bool			res_req;
		bool			res_res;
		bool			online;
		std::time_t		delay;
		pthread_mutex_t	mutex;
}; //Ping

#endif