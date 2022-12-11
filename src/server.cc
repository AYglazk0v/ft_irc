#include "../includes/Server.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>

Server::Server(char** argv) {
	if (!std::all_of(argv[1], argv[1] + std::strlen(argv[1]), [](unsigned char c){return std::isdigit(c);})) {
		error("Error: port incorrect. Use only digit");
	}
	
	port_ = atoi(argv[1]);
	if (port_ < 1024 || port_ > 49151) {
		error("Error: Wrong port. User 1024 - 49151");
	}
	
	password_.assign(argv[2], std::strlen(argv[2]));
	user_connection_ = 0;
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

void Server::new_user_connect() {
	int new_socket;
	int size_addr =  sizeof(addr_);

	if ((new_socket = accept(socket_, (struct sockaddr*)&addr_, (socklen_t*)&size_addr)) < 0 ) {
		throw std::runtime_error("Error create new user socket");
	}
	fcntl(new_socket, F_SETFL, O_NONBLOCK);
	FD_SET(new_socket, &read_);
	req_res[user_connection_].client_sock_fd = new_socket;
	req_res[user_connection_].disconnect = 0;
	users_.push_back(new User(user_connection_, new_socket, addr_));
	user_connection_++;

	std::cout << "\nNew connection id[" << user_connection_ - 1 << "], open socket: " << new_socket << std::endl;
	std::cout << "Ip adress: " << inet_ntoa(addr_.sin_addr) << std::endl;
	std::cout << "Port: " << ntohs(addr_.sin_port) << std::endl;
}

void Server::loop(){
	int			max_sock_fd;
	int			activity;
	pthread_t	checking_ping;

	while (true) {
		FD_ZERO(&read_);
		FD_SET(socket_, &read_);
		max_sock_fd = socket_;
		
		for(auto&& usr : users_) {
			if (usr->getSockFd() > 0) {
				FD_SET(usr->getSockFd(), &read_);
			}
			if (usr->getSockFd() > max_sock_fd) {
				max_sock_fd = usr->getSockFd();
			}
		}

		activity = select(max_sock_fd + 1, &read_, NULL, NULL, NULL);
		if ((activity < 0) && (errno != EINTR)) {
			std::cerr<< "timeout select" << std::endl;
		}
		if (FD_ISSET(socket_, &read_)) {
			try {
				new_user_connect();
			} catch (const std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
		} else {
			start_read();
		}
	}
}

void Server::start_read() {
	int read_b;
	int size_addr;
	char buff[1025];

	size_addr = sizeof(addr_);
	for (auto&& usr : users_) {
		int sock_fd = usr->getSockFd();
		if (req_res[usr->getId()].disconnect == 1) {
			std::cout << "DISCONNECT!" << std::endl;
			disconnect(usr);
			return;
		}
		if (FD_ISSET(sock_fd, &read_)) {
			if ((read_b = read(sock_fd, &buff, 1024)) < 0) {
				std::cerr << "Error: reading from socket [" << sock_fd << "]" << std::endl;
				return;
			} else if (read_b == 0) {
				disconnect(usr);
				return;
			} else {
				if (!(usr->createBuff(read_b, buff))) {
					return;
				}
				std::cout << "New Buff :" << usr->getBuff() << std::endl;
				if (usr->getAutorization() && !req_res[usr->getId()].res_wait) {
					req_res[usr->getId()].res_req = 1;
				}
				// if (parseMessage(usr)) {
					// return;
				// }
			}
		}
	}
}

void Server::disconnect(User* usr) {
	history_.push_back(usr);
	for (std::vector<Chanel *>::iterator it = chanels.begin(), ite = chanels.end(); it != ite; ++it) {
		if ((*it)->isMember(usr)) {
			if ((*it)->isOperators(usr) && (*it)->memberSize() > 1 && (*it)->operSize() == 1) {
				(*it)->addOper(usr, NULL);
			} else if ((*it)->memberSize() == 1) {
				chanels.erase(it);
				break;
			}
			(*it)->removeOperator(usr);
			(*it)->removeMember(usr);
			(*it)->sendAll(usr, "QIUT", "Client exit", "");
			break;
		}
	}
	std::cout << "\nUser disconnected, ip: " << inet_ntoa(usr->getAddr().sin_addr) << " port: " << ntohs(usr->getAddr().sin_port) << std::endl;
	req_res[usr->getId()].online = 0;
	close(usr->getSockFd());
	// users_.erase(std::remove_if(users_.begin(), users_.end(), [&usr](User& tmp) {return tmp.getId() == usr->getId();})); ЕСЛИ БЫ МЫ НЕ ЗНАЛИ, ЧТО ИДЕМ по USERS_
	users_.erase(std::remove(users_.begin(), users_.end(), usr));
}

void Server::compileMsg(User &sendler, User &recipient, std::string arg1, std::string arg2, std::string arg3) {
	std::string msg = ":" + sendler.getNick() + "!" + sendler.getUsername() + "@" + std::string(inet_ntoa(sendler.getAddr().sin_addr)) + " ";
	if (arg2.empty()) {
		msg += arg1 + "\r\n";
	} else if (arg3.empty()) {
		msg += arg1 + " :" + arg2 + "\r\n";
	} else {
		msg += arg1 + " " + arg2 + " :" + arg3 + "\r\n";
	}
	std::cout << "SEND MSG SOCKET_FD:" << recipient.getSockFd() << "\n" << msg << std::endl;
	sendMsg(recipient.getSockFd(), msg);
}

bool Server::sendMsg(int sock_fd, const std::string& msg) {
	if ((send(sock_fd, msg.c_str(), msg.length(), 0)) >= 0) {
		return true;
	}
	std::cerr << "Error occurred while writing to socket" << std::endl;
	return false;
}