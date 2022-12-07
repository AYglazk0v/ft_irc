#ifndef USER_HPP
# define USER_HPP

class User {
	private:
		int					id_;
		int 				sock_fd;
		struct sockaddr_in	addres_;
	public:
		User(int id, int sock_fd, sockaddr_in addres);
};//User

#endif