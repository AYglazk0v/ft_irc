#include "../includes/Server.hpp"

namespace {

}

Server::Server(char** argv) {
	if (!std::all_of(argv[1], argv[1] + std::strlen(argv[1]), [](unsigned char c){return std::isdigit(c);})) {
		error("Error: port incorrect. Use only digit");
	}
	
	port_ = atoi(argv[1]);
	if (port_ < 1024 || port_ > 49151) {
		error("Error: Wrong port. User 1024 - 49151");
	}
	
	password_.assign(argv[2], std::strlen(argv[2]));
	user_connectin_ = 0;
	FD_ZERO(&read_);
	FD_ZERO(&write_);
	FD_ZERO(&accept_);
	time_.tv_sec = 0;
	time_.tv_usec = 0;
	
	for (auto&& tmp : req_res) {
		pthread_mutex_init(&tmp.mutex, NULL);
		tmp.delay = -1;
		tmp.online = 1;
	}
	
	socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ < 0) {
		error("Error: create socket");
	}
	
	int enable = 1;
	if ((setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable))) < 0 ) {
		error("Error: setsockopt failed");
	}

	addr_.sin_family = AF_INET;
	addr_.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_.sin_port = htons(port_);

	if ((bind(socket_, (struct sockaddr*)&addr_, sizeof(addr_))) < 0) {
		error("Error: bind socket");
	}

	if ((listen(socket_, SOMAXCONN)) < 0) {
		error("Error: listen socket");
	}
	fcntl(socket_, O_NONBLOCK);
	std::cout << "IRC SERVER STARTED. \nPORT" << port_ << "\tPassword: " + password_ + "\nWaiting for connection ..." << std::endl;
}