#ifndef USER_HPP
# define USER_HPP

# include "Server.hpp"

class User {
	private:
		int					id_;
		int 				sock_fd_;
		struct sockaddr_in	addres_;

		bool				access_;
		bool				autorization_;
		bool				away_;
		bool				oper_;
		bool				invis_;
		
		std::string			nick_;
		std::string			user_;
		std::string			host_;
		std::string			serverName_;
		std::string			realName_;
		std::string			buff_;
		std::string			away_msg_;

	public:
		User(int id, int sock_fd, sockaddr_in addres);
		int getSockFd() { return sock_fd_; }

};//User

#endif