#include "../includes/User.hpp"

User::User(int id, int sock_fd, sockaddr_in addres) : id_(id), sock_fd_(sock_fd), addres_(addres) {

}