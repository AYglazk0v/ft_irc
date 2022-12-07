#ifndef USER_HPP
# define USER_HPP

# include "Server.hpp"

class User {
	private:
		int					id_;
		int 				sock_fd_;
		struct sockaddr_in	addres_;

	public:
		User(int id, int sock_fd, sockaddr_in addres);
		int getSockFd() { return sock_fd_; }
		
};//User

#endif