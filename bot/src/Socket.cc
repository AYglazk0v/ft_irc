#include "../includes/Socket.hpp"

Socket::Socket() : fd_(0), host_(IRC_SERVER), port_(IRC_SPORT) {
	if ((fd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cerr << "Cannot create socket" << std::endl;
		exit(EXIT_FAILURE);
	}
	set();
}

void Socket::set() {
	addr_.sin_family = AF_INET;
	addr_.sin_port = htons(port_);
	addr_.sin_addr.s_addr = inet_addr(host_.c_str());
	if (addr_.sin_addr.s_addr == static_cast<in_addr_t>(-1)) {
		std::cerr << "Cannot transform socket address" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Socket::tryToConnect() {
	if ((connect(fd_, (struct sockaddr *)&addr_, sizeof(addr_))) < 0) {
		std::cerr << "Cannot with this socket" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int Socket::tryToSend(const std::string &msg) {
	if ((send(fd_, msg.c_str(), msg.length(), MSG_NOSIGNAL)) < 0) {
		std::cerr << "Cannot send data" << std::endl;
		exit(EXIT_FAILURE);
	}
	return 0;
}

std::string Socket::tryToRecv() {
	char buf[SOCK_BUFF_SIZE] = {};
	std::string ret;
	int read = 0;
	while ((read = recv(fd_, buf, SOCK_BUFF_SIZE - 1, 0)) > 0) {
		buf[read] = 0;
		ret += buf;
	}
	return ret;
}