#include "../includes/Socket.hpp"

std::string exec(const std::string& cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

int main() {
	int port = IRC_SPORT;
	std::unique_ptr<Socket> ircSocket = std::make_unique<Socket>();
	ircSocket->tryToConnect();
	fcntl(ircSocket->getFd(), F_SETFL, O_NONBLOCK);
	std::string msg = "PASS " + static_cast<std::string>(IRC_SPASS) + "\n";
	msg += "NICK " + static_cast<std::string>(IRC_NICK) + "\n";
	msg += "USER " + static_cast<std::string>(IRC_USER) + " " + static_cast<std::string>(IRC_HOST) + " " + static_cast<std::string>(IRC_SNAME) + " " + static_cast<std::string>(IRC_RNAME) + "\r\n";
	ircSocket->tryToSend(msg);
	while (true) {
		msg = "";
		msg = ircSocket->tryToRecv();
		if (msg != "") {
			std::cout << msg;
			if (msg.find("PRIVMSG") != std::string::npos) {
				std::string target = msg.substr(1, msg.find("PRIVMSG") - 1);
				std::string check_recv = msg.substr(msg.rfind(':') + 2, msg.size() - msg.rfind(':'));
				if (!check_recv.empty() && check_recv.size() > 2) {
					check_recv.pop_back();
					check_recv.pop_back();
				} else {
					continue;
				}
				std::cout << "CHECK: " << check_recv<< std::endl; 
				if (std::all_of(check_recv.begin(), check_recv.end(), [](char c){return std::isalpha(c);})) {
					msg = "curl -s 'wttr.in/" + check_recv + "?0'";
					msg = exec(msg);
					size_t it = 0;
					auto ite = msg.find('\n');
					while ( ite != std::string::npos) {
						auto read = "PRIVMSG " + target + " " + msg.substr(it, ite) + "\r\n";
						ircSocket->tryToSend(read.c_str());
						it = ite + 1;
						ite = msg.find('\n', it);
					}
				}
			} else if (msg.find("PING") != std::string::npos) {
				msg = msg.substr(msg.rfind(':') + 1, msg.size() - msg.rfind(':'));
				msg = "PONG" + msg;
				ircSocket->tryToSend(msg);
			}
		}
	}
}