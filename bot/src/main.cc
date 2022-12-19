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
	msg += "USER " + static_cast<std::string>(IRC_USER) + "\n";
	msg += "NICK " + static_cast<std::string>(IRC_NICK) + "\r\n";
	ircSocket->tryToSend(msg);
	while (true) {
		msg = "";
		msg = ircSocket->tryToRecv();
		if (msg != "") {
			std::cout << msg;
			if (msg.find("PRIVMSG") != std::string::npos) {
				std::string target = msg.substr(1, msg.find('!') - 1);
				std::string check_recv = msg.substr(msg.rfind(':') + 1, msg.size() - msg.rfind(':'));
				check_recv.pop_back();
				check_recv.pop_back();
				if (std::all_of(check_recv.begin(), check_recv.end(), [](char c){return std::isalpha(c);})) {
					msg = "curl -s 'wttr.in/" + check_recv + "?0";
					msg = exec(msg);
					msg = "PRIVMSG " + target + " " + msg;
					ircSocket->tryToSend(msg);
				}
			} else if (msg.find("PING") != std::string::npos) {
				msg = msg.substr(msg.rfind(':') + 1, msg.size() - msg.rfind(':'));
				msg = "PONG" + msg;
				ircSocket->tryToSend(msg);
			}
		}
	}
}