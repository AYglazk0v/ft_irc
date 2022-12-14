#include "../includes/Commands.hpp"
#include <bitset>
#include <fstream>

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
	if (std::any_of(users.begin(), users.end(), [&args](User*& us){return us->getNick() == args[1];})) {
		msg = compileError(433, *user, args[1], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	if (std::any_of(args[1].begin(), args[1].end(),[](const char c){return c <= 32 || c > 126 || c == ' ' || c == ':' || c == '#' || c == '!' || c == '@';})) {
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
	std::string msg = "";
	if (args.size() <= 1) {
		msg = compileError(461, *user, args[1], "");
		Server::sendMsg(user->getSockFd(), msg);
	} else {
		for (auto&& curr_arg : args) {
			auto j = std::find_if(users.begin(), users.end(), [&curr_arg](User*& us){ return curr_arg == us->getNick();});
			if (j != users.end()) {
				msg += curr_arg + " ";
			}
		}
		if (!msg.empty()) {
			msg.pop_back();
		}
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

void Commands::cmd_info(std::vector<std::string> args, User *&user) {
	std::string msg;
	if (args.size() <= 1 || (args.size() != 1 && args[1] == "IRC_SERVER")) {
		std::vector<std::string> repl_msgs = {"_______SERVER_INFORMATION________"};
		msg = compileReply(371, *user, repl_msgs);
		Server::sendMsg(user->getSockFd(), msg);
		repl_msgs[0] = "_this is the best ir server mayb_";
		msg = compileReply(371, *user, repl_msgs);
		Server::sendMsg(user->getSockFd(), msg);
		repl_msgs[0] = "_________________________________";
		msg = compileReply(371, *user, repl_msgs);
		Server::sendMsg(user->getSockFd(), msg);
	} else {
		msg = compileError(402, *user, args[1], "");
		Server::sendMsg(user->getSockFd(), msg);
	}
}

void Commands::cmd_away(std::vector<std::string> args, User *&user) {
	std::string msg;
	std::vector<std::string> repl_msgs = {};
	if (args.size() <= 1) {
		user->setAway(0);
		msg = compileReply(305, *user, repl_msgs);
		Server::sendMsg(user->getSockFd(), msg);
	} else {
		user->setAway(1);
		for (std::vector<std::string>::iterator it = args.begin(), ite = args.end(); it != ite; ++it) {
			msg += *it + " ";
		}
		if (!msg.empty()) {
			msg.pop_back();
		}
		user->setAwayMsg(msg);
		msg = compileReply(306, *user, repl_msgs);
		Server::sendMsg(user->getSockFd(), msg);
	}
}

void Commands::cmd_privmsg(std::vector<std::string> args, User *&user, std::vector<User *> &users, std::vector<Chanel *> &chanels, bool notice) {
	std::string msg;
	if (args.size() == 1) {
		msg = compileError(411, *user, args[0], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	if (args.size() == 2) {
		msg = compileError(412, *user, "", "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	std::vector<std::string> target = split(args[1], ',');
	std::vector<std::string> repl_msgs = {};
	for (auto&& curr_target : target) {
		if (curr_target.front() != '#' && curr_target.front() != '&') {
			auto it = std::find_if(users.begin(), users.end(), [&curr_target](User*& us){return curr_target == us->getNick();});
			if (it != users.end()) {
				msg.clear();
				msg += ":" + user->getNick() + " PRIVMSG " + (*it)->getNick() + ": ";
				for (std::vector<std::string>::iterator itt = args.begin() + 2, itte = args.end(); itt != itte; ++itt) {
					msg += *itt + " ";
				}
				if (!msg.empty()) {
					msg.pop_back();
					msg += "\r\n";
				}
				Server::sendMsg((*it)->getSockFd(), msg);
				if ((*it)->getAway() && !notice) {
					repl_msgs.push_back((*it)->getNick());
					repl_msgs.push_back((*it)->getAwayMsg());
					msg = compileReply(301, *user, repl_msgs);
					Server::sendMsg(user->getSockFd(), msg);
				}
			} else {
				msg = compileError(401, *user, "", "");
				Server::sendMsg(user->getSockFd(), msg);
			}
		} else {
			bool send = 0;
			auto it = std::find_if(chanels.begin(), chanels.end(), [&curr_target](Chanel*& ch){return  curr_target == ch->getName(); });
			if (it != chanels.end()) {
				if ((*it)->isBanned(user) || (*it)->isMember(user)) {
					msg = compileError(404, *user, "", "");
					Server::sendMsg(user->getSockFd(), msg);
					continue;
				}
				auto itt = std::find_if(users.begin(), users.end(), [&it, &user](User*& curr_us){return (user->getNick() != curr_us->getNick() && (*it)->isMember(curr_us));});
				if (itt != users.end()) {
					msg.clear();
					for (std::vector<std::string>::iterator it = args.begin() + 2, ite = args.end(); it != ite; ++it) {
						msg += *it + " ";
					}
					Server::compileMsg(*user, *(*itt), args[0], args[1], msg);
				}
				send = 1;
			}
			else if (!send && it == chanels.end()) {
				msg = compileError(401, *user, "", "");
				Server::sendMsg(user->getSockFd(), msg);
			}
		}
	}
}

void Commands::cmd_join(std::vector<std::string> args, User *&user, std::vector<User *> &users, std::vector<Chanel *> &chanels) {
	std::string msg;
	std::vector<std::string> arr_channel;
	std::vector<std::string> arr_password;
	if (args.size() <= 1) {
		compileError(461, *user, args[0], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	arr_channel = split(args[1], ',');
	if (args.size() > 2) {
		arr_password = split(args[2], ',');
	}
	while (arr_password.size() < arr_channel.size()) {
		arr_password.push_back("");
	}
	for ( auto it = arr_channel.begin(), ite = arr_channel.end(); it != ite; ++it) {
		if ((*it).front() != '#') {
			msg = compileError(403, *user, *it, "");
			Server::sendMsg(user->getSockFd(), msg);
			continue;
		}
		for (auto itt = (*it).begin(), itte = (*it).end(); itt != itte; ++itt) {
			if ((*itt) <= 32 || (*itt) > 126 || (*itt) == ':' || (*itt) == '!' || (*itt) == ' ' || (*itt) == '@') {
				msg = compileError(403, *user, *it, "");
				Server::sendMsg(user->getSockFd(), msg);
				continue;
			}
		}
		bool new_chanel = 1;
		for (auto it_chanel = chanels.begin(), ite_chanel = chanels.end(); it_chanel != ite_chanel; ++it_chanel) {
			if ((*it_chanel)->getName() == (*it)) {
				new_chanel = 0;
				if ((*it_chanel)->isBanned(user)) {
					msg = compileError(474, *user, *it, "");
					Server::sendMsg(user->getSockFd(), msg);
					continue;
				} else if ((*it_chanel)->isMember(user)) {
					msg = compileError(443, *user, *it, "");
					Server::sendMsg(user->getSockFd(), msg);
					continue;
				} else if ((*it_chanel)->isInvited(user)) {
					msg = compileError(473, *user, *it, "");
					Server::sendMsg(user->getSockFd(), msg);
					continue;
				} else if ((*it_chanel)->memberSize() >= (*it_chanel)->max_member()) {
					msg = compileError(471, *user, *it, "");
					Server::sendMsg(user->getSockFd(), msg);
					continue;
				} else if (!(*it_chanel)->getPassword().empty() && (*it_chanel)->getPassword() != arr_password[it - arr_channel.begin()]) {
					msg = compileError(475, *user, *it, "");
					Server::sendMsg(user->getSockFd(), msg);
					continue;
				}
				(*it_chanel)->addUser(user);
				(*it_chanel)->sendAll(user, args[0], *it, "");
				std::vector<std::string> repl_msgs = {(*it), (*it_chanel)->getTopic()};
				if (!(*it_chanel)->getTopic().empty()) {
					msg = compileReply(332, *user, repl_msgs);
					Server::sendMsg(user->getSockFd(), msg);
				} else {
					if (!repl_msgs.empty()) {
						repl_msgs.pop_back();
					}
					msg = compileReply(331, *user, repl_msgs);
					Server::sendMsg(user->getSockFd(), msg);
				}
				repl_msgs.clear();
				repl_msgs.push_back("= " + (*it));
				repl_msgs.push_back((*it_chanel)->getOperNames() + " " + (*it_chanel)->getUserNames());
				msg = compileReply(353, *user, repl_msgs);
				Server::sendMsg(user->getSockFd(), msg);
				repl_msgs[0] = *it;
				msg = compileReply(366, *user, repl_msgs);
				Server::sendMsg(user->getSockFd(), msg);
			}
		}
		if (new_chanel) {
			Chanel *tmp = new Chanel(user, *it, arr_password[it - arr_password.begin()]);
			chanels.push_back(tmp);
			tmp->sendAll(user, args[0], *it, "");
			std::vector<std::string> repl_msgs = {(*it)};
			msg = compileReply(331, *user, repl_msgs);
			Server::sendMsg(user->getSockFd(), msg);
			repl_msgs[0] = "= " + (*it);
			repl_msgs.push_back("@" + user->getNick());
			msg = compileReply(353, *user, repl_msgs);
			Server::sendMsg(user->getSockFd(), msg);
			repl_msgs[0] = *it;
			msg = compileReply(366, *user, repl_msgs);
			Server::sendMsg(user->getSockFd(), msg);
		}
	}
}

void Commands::cmd_invite(std::vector<std::string> args, User *&user, std::vector<User *> &users, std::vector<Chanel *> &chanels) {
	std::string msg;
	if (args.size() != 3) {
		msg = compileError(461, *user, args[0], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	User *target = nullptr;
	auto check_nick = find_if(users.begin(), users.end(), [&args](User*& curr_user){return args[1] == curr_user->getNick();});
	if (check_nick != users.end()) {
		target = *check_nick;
	} else {
		msg = compileError(401, *user, args[1], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	Chanel *target_ch = nullptr;
	auto check_chanel = std::find_if(chanels.begin(), chanels.end(), [&args](Chanel*& curr_ch){return args[2] == curr_ch->getName();});
	if (check_chanel != chanels.end()) {
		target_ch = *check_chanel;
	} else {
		msg = compileError(442, *user, args[2], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	if (target_ch->isMember(target)) {
		msg = compileError(443, *user, args[1], args[2]);
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	if (target_ch->getInvOnly() && !target_ch->isOperators(user)) {
		msg = compileError(482, *user, args[2], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	if (!target->getAway()) {
		if (target_ch->isInvited(target)) {
			target_ch->addInvitedUser(target);
		}
		std::vector<std::string> repl_msgs = {args[2], args[1]};
		msg = compileReply(341, *target, repl_msgs);
		Server::sendMsg(user->getSockFd(), msg);
		Server::compileMsg(*user, *target, args[0] + " " + target->getNick() + " " + args[2], "", "");
	} else {
		std::vector<std::string> repl_msgs = {args[1], target->getAwayMsg()};
		msg = compileReply(301, *user, repl_msgs);
		Server::sendMsg(user->getSockFd(), msg);
	}
}

std::string Commands::Chanel_who(User *&user, std::vector<Chanel *> &chanels) {
	std::string msg = {};
	for (auto it_chanel = chanels.begin(), ite_chanel = chanels.end(); it_chanel != ite_chanel; ++it_chanel) {
		if ((*it_chanel)->isMember(user)) {
			if ((*it_chanel)->getInvOnly()) {
				msg += "Prv,";
			} else {
				msg += (*it_chanel)->getName() + " ";
			}
		}
	}
	if (!msg.empty()) {
		msg.pop_back();
	} else {
		msg = "*no joined channels*";
	}
	return msg;
}

void Commands::cmd_who(std::vector<std::string> args, User *&user, std::vector<User *> &users, std::vector<Chanel *> &chanels) {
	std::string msg;
	bool oper = 0;
	if (args.size() <= 1) {
		return;
	}
	if ((args.size() > 2 && args[2] == "o") || (args.size() == 2 && args[1] == "o")) {
		oper = 1;
	}
	if (args.size() > 1 && args[1] != "O") {
		bool status = 0;
		for (auto&& curr_ch : chanels) {
			if (curr_ch->getName() == args[1]) {
				status = 1;
				for (auto it = users.begin(), ite = users.end(); it != ite; ++it) {
					if (curr_ch->isMember(*it)) {
						msg = Chanel_who(*it, chanels);
						std::vector<std::string> repl_msgs = {args[1]};
						msg = compileReply(352, *user, repl_msgs);
						Server::sendMsg(user->getSockFd(), msg);
					}
				}
			}
		}
		if (!status) {
			msg = compileError(403, *user, args[0], "");
			Server::sendMsg(user->getSockFd(), msg);
		} else {
			std::vector<std::string> repl_msgs = {args[1]};
			msg = compileReply(315, *user, repl_msgs);
			Server::sendMsg(user->getSockFd(), msg);
		}
	} else {
		for (auto&& it = users.begin(), ite = users.end(); it != ite; ++it) {
			if ((!(*it)->getInvis() && (!oper || (oper && (*it)->getOper()))) 
				|| ((*it)->getInvis() && (*it)->getNick() == user->getNick() && (!oper || (oper && (*it)->getOper())))) {
				msg = Chanel_who(*it, chanels);
				std::vector<std::string> repl_msgs = {msg, (*it)->getUsername(), (*it)->getHost(), (*it)->getServerName(), (*it)->getNick(), "H", "", (*it)->getRealName()};
				msg = compileReply(352, *user, repl_msgs);
				Server::sendMsg(user->getSockFd(), msg);
			}
		}
		std::vector<std::string> repl_msgs = {};
		msg = compileReply(315, *user, repl_msgs);
		Server::sendMsg(user->getSockFd(), msg);
	}
}

void Commands::cmd_part(std::vector<std::string> args, User *&user, std::vector<User *> &users, std::vector<Chanel *> &chanels) {
	std::string msg;
	if (args.size() < 2) {
		msg = compileError(461, *user, args[0], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	bool exit = 0;
	for (auto&& curr_ch : chanels) {
		if (curr_ch->getName() == args[1]) {
			if (curr_ch->isMember(user)) {
				if (curr_ch->isOperators(user) && curr_ch->memberSize() > 1 && curr_ch->operSize() == 1) {
					curr_ch->addOper(user, nullptr);
				} else if (curr_ch->memberSize() == 1) {
					chanels.erase(std::remove(chanels.begin(), chanels.end() ,curr_ch));
					break;
				}
				curr_ch->removeOperator(user);
				curr_ch->removeMember(user);
				curr_ch->sendAll(user, "QUIT", "Client exit", "");
				exit = 1;
				for (auto&& curr_user : users) {
					if (curr_ch->isMember(curr_user)) {
						msg = Chanel_who(user, chanels);
						std::vector<std::string> repl_msgs = {"= " + args[1], curr_ch->getOperNames() + " " + curr_ch->getUserNames()};
						msg = compileReply(353, *curr_user, repl_msgs);
						Server::sendMsg(curr_user->getSockFd(), msg);
						repl_msgs[0] = args[1];
						msg = compileReply(366, *curr_user, repl_msgs);
						Server::sendMsg(curr_user->getSockFd(), msg);
					}
				}
				break;
			} else {
				msg = compileError(442, *user, args[1], "");
				Server::sendMsg(user->getSockFd(), msg);
			}
		}
	}
	if (!exit) {
		msg = compileError(403, *user, args[1], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
}

void Commands::cmd_whois(std::vector<std::string> args, User *&user, std::vector<User *> &users, std::vector<Chanel *> &chanels) {
	std::string msg;
	if (args.size() < 2) {
		msg = compileError(431, *user, "", "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	std::vector<std::string> user_list = split(args[1], ',');
	for (auto&& curr_user : user_list) {
		auto it = std::find_if(users.begin(), users.end(), [&curr_user](User*& find_user){return curr_user == find_user->getNick();});
		if (it != users.end()) {
			std::vector<std::string> repl_msgs = {(*it)->getNick(), (*it)->getUsername(), (*it)->getHost(), (*it)->getRealName()};
			msg = compileReply(311, *user, repl_msgs);
			Server::sendMsg(user->getSockFd(), msg);
			repl_msgs[1] = "AFK MODE";
			msg = compileReply(317, *user, repl_msgs);
			Server::sendMsg(user->getSockFd(), msg);
			if ((*it)->getAway()) {
				repl_msgs[1] = (*it)->getAwayMsg();
				msg = compileReply(301, *user, repl_msgs);
				Server::sendMsg(user->getSockFd(), msg);
			}
			if ((*it)->getOper()) {
				msg = compileReply(313, *user, repl_msgs);
				Server::sendMsg(user->getSockFd(), msg);
			}
			msg = "";
			for (auto&& curr_ch : chanels) {
				if (curr_ch->isMember(*it)) {
					msg += curr_ch->getName() + " ";
				}
			}
			if (!msg.empty()) {
				msg.pop_back();
				msg = compileReply(319, *user, repl_msgs);
				Server::sendMsg(user->getSockFd(), msg);
			}
			msg = compileReply(318, *user, repl_msgs);
			Server::sendMsg(user->getSockFd(), msg);
		}
	}
}

void Commands::cmd_topic(std::vector<std::string> args, User *&user, std::vector<User *> &users, std::vector<Chanel *> &chanels) {
	std::string msg;
	if (args.size() < 2) {
		msg = compileError(431, *user, args[0], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	auto it = std::find_if(chanels.begin(), chanels.end(), [&args](Chanel*& curr_ch){return  args[1] == curr_ch->getName();});
	if (it != chanels.end()) {
		if (args.size() == 2) {
			if (!(*it)->getTopic().empty()) {
				std::vector<std::string> repl_msgs = {args[1], (*it)->getTopic()};
				msg = compileReply(332, *user, repl_msgs);
				Server::sendMsg(user->getSockFd(), msg);
			} else {
				std::vector<std::string> repl_msgs = {args[1]};
				msg = compileReply(331, *user, repl_msgs);
				Server::sendMsg(user->getSockFd(), msg);
			}
		} else {
			msg = "";
			for (int i = 2; i < args.size(); ++i) {
				msg += args[i] + " ";
			}
			(*it)->setTopic(msg);
			std::for_each(users.begin(), users.end(),
				[&user, &args, &msg](User*& curr_user){ Server::compileMsg(*user, *curr_user, args[0], args[1], msg);});
		}
	}
}

void Commands::cmd_kick(std::vector<std::string> args, User *&user, std::vector<User *> &users, std::vector<Chanel *> &chanels) {
	std::string msg;
	if (args.size() < 3) {
		msg = compileError(461, *user, args[0], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	std::vector<std::string> target_ch = split(args[1],	',');
	std::vector<std::string> target_user = split(args[2], ',');
	bool chanel = 0, userb = 0, kick = 0;
	std::vector<User* > del_user;
	for (auto&& t_c : target_ch) {
		auto it_find_ch = std::find_if(chanels.begin(), chanels.end(), [&t_c](Chanel* ch) {return t_c == ch->getName();});
		if (it_find_ch != chanels.end()) {
			chanel = 1;
			for (auto t_u : target_user) {
				for (auto it = users.begin(), ite = users.end(); it != ite; ++it) {
					if (t_u != (*it)->getNick()) {
						userb = 1;
					}
					if (t_u == (*it)->getNick() && (*it_find_ch)->isMember(*it)) {
						userb = 1;
						kick = 1;
						if ((*it_find_ch)->isOperators(*it) || !(*it_find_ch)->isOperators(user)) {
							msg = compileError(482, *user, t_c, "");
							Server::sendMsg(user->getSockFd(), msg);
							continue;
						}
						del_user.push_back(*it);
					}
					if (!userb) {
						msg = compileError(442, *user, t_c, "");
						Server::sendMsg(user->getSockFd(), msg);
					}
					userb = 0;
				}
				if (!kick){
					msg = compileError(401, *user, t_u, "");
					Server::sendMsg(user->getSockFd(), msg);
				}
				kick = 0;
			}
		}
		if (!chanel) {
			msg = compileError(403, *user, t_c, "");
			Server::sendMsg(user->getSockFd(), msg);
		}
		chanel = 0;
	}
	if (args.size() > 3) {
		msg = "";
		for (auto&& curr_arg : args) {
			msg += curr_arg + " ";
		}
	} else {
		msg = user->getNick();
	}
	while (del_user.size()) {
		for (auto&& curr_ch : chanels) {
			if (curr_ch->isMember(del_user[0])) {
				std::vector<User*>::iterator it_u;
				for (std::vector<User *>::iterator it = del_user.begin(), ite = del_user.end(); it != ite; ++it) {
					if ((*it)->getNick() == del_user[0]->getNick()) {
						it_u = it;
					}
				}
				curr_ch->sendAll(user, args[0], curr_ch->getName() + " " + (*it_u)->getNick(), msg);
				curr_ch->removeMember(del_user[0]);
				del_user.erase(it_u);
				break;
			}
		}
	}
}

void Commands::cmd_mode(std::vector<std::string> args, User *&user, std::vector<User *> &users, std::vector<Chanel *> &chanels) {
	std::string msg;
	if (args.size() < 3) {
		msg = compileError(461, *user, args[0] , "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	if (args[2][0] != '+' && args[2][0] != '-') {
		msg = compileError(472, *user, args[2] , "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	std::string flags_c = "opsitnmlbvk";
	std::string flags_u = "iswo";
	if (args[2].size() > 2 && (args[2].find_first_of("olv") != std::string::npos)) {
		msg = compileError(472, *user, args[2], "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
	std::bitset<255> flags = {};
	for (int i = 1; i < args[2].length(); ++i) {
		if ((args[1][0] == '#' && flags_c.find(args[2][i]) != std::string::npos) || (args[1][0] != '#' && flags_u.find(args[2][i]) != std::string::npos)) {
			if (flags[args[2][i]] == 1) {
				msg = compileError(501, *user, args[2], "");
				Server::sendMsg(user->getSockFd(), msg);
				return;
			}
			flags[args[2][i]] = 1;
		}
	}
	if (args[1][0] == '#') {
		if (args.size() < 4) {
			msg = compileError(461, *user, args[0], "");
			Server::sendMsg(user->getSockFd(), msg);
			return;
		}
		auto target_user = std::find_if(users.begin(), users.end(), [&args](User*& curr_user){return curr_user->getNick() == args[3];});
		if (args[3].find("*!") != std::string::npos) {
			target_user = std::find_if(users.begin(), users.end(), [&args](User*& curr_user){return args[3].find(curr_user->getUsername()) != std::string::npos;});
		}
		auto target_ch = std::find_if(chanels.begin(), chanels.end(), [&args](Chanel*& curr_ch){return  args[1] == curr_ch->getUserNames();});
		if (target_ch != chanels.end() && target_user != users.end()) {
			if (!(*target_ch)->isOperators(user)) {
				msg = compileError(482, *user, args[1], "");
				Server::sendMsg(user->getSockFd(), msg);
				return;
			}
			for (auto&& c : args[2]){
				if (c == 'o' && args[2][0] == '+') {
					(*target_ch)->addOper(user, *target_user);
				} else if (c == 'o' && args[2][0] == '-') {
					if ((*target_ch)->memberSize() == 1) {
						return;
					}
					(*target_ch)->removeOperator(*target_user);
					if ((*target_ch)->operSize() == 0) {
						(*target_ch)->addOper(user, nullptr);
					}
					(*target_ch)->sendAll(user, "MODE " + args[1] + " -o " + (*target_user)->getNick(), "", "");
				} else if (c == 'v' && args[2][0] == '+') {
					(*target_ch)->sendAll(user, "MODE " + args[1] + " +v " + (*target_user)->getNick(), "", "");
				} else if (c == 'v' && args[2][0] == '-') {
					(*target_ch)->sendAll(user, "MODE " + args[1] + " -v " + (*target_user)->getNick(), "", "");
				} else if (c == 'b' && args[2][0] == '+') {
					(*target_ch)->sendAll(user, "MODE " + args[1] + " +b " + (*target_user)->getNick(), "", "");
					(*target_ch)->addBan(*target_user);
				} else if (c == 'b' && args[2][0] == '-') {
					(*target_ch)->sendAll(user, "MODE " + args[1] + " -b " + (*target_user)->getNick(), "", "");
					(*target_ch)->removeBan(*target_user);
				}
			} 
		} else {
			msg = compileError(403, *user, args[1], "");
			Server::sendMsg(user->getSockFd(), msg);
			return;
		}
	} else if (user->getNick() == args[1]) {
		for (int i = 1; i < args[2].length(); ++i) {
			if (args[2][i] == 'i' && args[2][0] == '+') {
				user->setInvis(1);
			} else if (args[2][i] == 'i' && args[2][0] == '-') {
				user->setInvis(0);
			} else if (args[2][i] == 's' && args[2][0] == '+') {
				user->setInvis(1);
			} else if (args[2][i] == 'w' && args[2][0] == '+') {
				user->setInvis(1);
			} else if (args[2][i] == 'o' && args[2][0] == '+') {
				user->setInvis(1);
			}
		}
	} else {
		msg = compileError(502, *user, "", "");
		Server::sendMsg(user->getSockFd(), msg);
		return;
	}
}