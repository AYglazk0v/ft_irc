#include "../includes/User.hpp"

User::User(int id, int sock_fd, sockaddr_in addres) : id_(id), sock_fd_(sock_fd), addres_(addres) {
	access_ = 0;
	autorization_ = 0;
	away_ = 0;
	oper_ = 0;
	invis_ = 0;
}