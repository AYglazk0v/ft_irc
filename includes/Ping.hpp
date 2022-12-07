#ifndef PING_HPP
# define PING_HPP

# include <ctime>

struct Ping {
		int				client_sock_fd;
		std::time_t		delay;
		pthread_mutex_t	mutex;
		bool			online;
		bool			disconnect;
		bool			res_wait;
		bool			res_req;
		bool			res_res;
}; //Ping

#endif