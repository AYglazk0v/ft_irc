#include "../includes/Commands.hpp"
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

int Commands::cmd_pass(std::vector<std::string> args, User* &user, Server *data) {
	std::string msg;
	if (args.size() < 2) {
		msg = compileError(411, *user, args[0], "");
		Server::sendMsg(user->getSockFd(), msg);
		return 0;
	}
	if (user->getAccess()) {
		msg = compileError(462, *user, "", "");
		data->sendMsg(user->getSockFd(), msg);
		return 0;
	}
	if (args[1] != data->getPassword()) {
		msg = compileError(464, *user, "", "");
		data->sendMsg(user->getSockFd(), msg);
		data->disconnect(user);
		return 1;
	} else {
		user->setAccess(1);
	}
	return 0;
}

void Commands::cmd_motd(User *&user) {
	std::ifstream file("motd/daily.msg");
	std::string msg;
	if (file) {
		std::vector<std::string> vec = {"IRS_SERVER"};
		msg = compileReply(375, *user, vec);
		Server::sendMsg(user->getSockFd(), msg);
		while (std::getline(file, vec[0])) {
			msg = compileReply(372, *user, vec);
			Server::sendMsg(user->getSockFd(), msg);
		}
		msg = compileReply(376, *user, vec);
		Server::sendMsg(user->getSockFd(), msg);
	} else {
		msg = compileError(422, *user, "", "");
		Server::sendMsg(user->getSockFd(), msg);
	}
}

void Commands::cmd_nick(std::vector<std::string> args, User *&user, std::vector<User *> &users) {
	std::string msg;
	if (args.size() != 2) {
		msg = compileError(461, *user, args[0], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	if (std::any_of(users.begin(), users.end(), [&args](const User& us){return us.getNick() == args[1];})) {
		msg = compileError(433, *user, args[1], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	if (std::any_of(args[1].begin(), args[2].end(),[](const char c){return c <= 32 || c > 126 || c == ' ' || c == ':' || c == '#' || c == '!' || c == '@';})) {
		msg = compileError(432, *user, args[1], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	int flag_first = {};
	if (user->getNick().empty()) {
		flag_first = 1;
	}
	user->setNick(args[1]);
	if (flag_first && user->getAutorization()) {
		cmd_motd(user);
	}
}

int Commands::cmd_user(std::vector<std::string> args, User* &user) {
	std::string msg;
	if (args.size() < 4) {
		msg =compileError(461, *user, args[0], "");
		Server::sendMsg(user->getSockFd(), msg);
		return 0;
	}
	if (user->getAutorization()) {
		msg = compileError(462, *user, "", "");
		Server::sendMsg(user->getSockFd(), msg);
		return 0;
	}
	user->setUser(args[1]);
	user->setHost(args[2]);
	user->setServerName(args[3]);
	user->setRealName(args[4]);
	user->setAutorizatuion(1);
	if (!user->getNick().empty()) {
		cmd_motd(user);
	}
	return 1;
}

int Commands::cmd_ping(std::vector<std::string> args, User *&user) {
	std::string msg;
	if (args.size() <= 1) {
		msg = compileError(409, *user, "", "");
		Server::sendMsg(user->getSockFd(), msg);
		return 0;
	}
	msg = ":IRC_SERVER PONG :" + args[1] + "\n"; 
	Server::sendMsg(user->getSockFd(), msg);
	return 1;
}

int Commands::cmd_pong(std::vector<std::string> args, User *&user) {
	std::string msg;
	if (args[1] == "IRC_SERVER") {
		return 1;
	}
	msg = compileError(402, *user, args[1], "");
	Server::sendMsg(user->getSockFd(), msg);
	return 0;
}

void Commands::cmd_ison(std::vector<std::string> args, User *&user, std::vector<User *> &users) {
	std::string msg;
	if (args.size() <= 1) {
		msg = compileError(461, *user, args[1], "");
		Server::sendMsg(user->getSockFd(), msg);
	} else {
		for (auto&& curr_arg : args) {
			auto j = std::find_if(users.begin(), users.end(), [&curr_arg](const User& us){ return curr_arg == us.getNick();});
			if (j != users.end()) {
				msg += curr_arg + " ";
			}
		}
		msg.pop_back();
		std::vector<std::string> repl_msgs = {msg};
		msg = compileReply(303, *user, repl_msgs);
		Server::sendMsg(user->getSockFd(), msg);
	}
}

void Commands::cmd_admin(std::vector<std::string> args, User *&user) {
	std::string msg;
	if (args.size() <= 1 || (args.size() != 1 && args[1] == "IRC_SERVER")) {
		std::vector<std::string> repl_msgs = {" IRC_SERVER"};
		msg = compileReply(256, *user, repl_msgs);
		Server::sendMsg(user->getSockFd(), msg);
		repl_msgs[0] = "gtaggana";
		msg = compileReply(257, *user, repl_msgs);
		Server::sendMsg(user->getSockFd(), msg);
		repl_msgs[0] = "Andrey";
		msg = compileReply(258, *user, repl_msgs);
		Server::sendMsg(user->getSockFd(), msg);
		repl_msgs[0] = "gtaggana@student.21-school.ru";
		msg = compileReply(259, *user, repl_msgs);
		Server::sendMsg(user->getSockFd(), msg);
	} else {
		msg = compileError(402, *user, args[1], "");
		Server::sendMsg(user->getSockFd(), msg);
	}
}