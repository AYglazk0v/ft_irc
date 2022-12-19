#include "../includes/Chanel.hpp"
#include "../includes/Server.hpp"
#include <algorithm>
#include <regex>

Chanel::Chanel(User *user, std::string name, std::string password) {
	name_ = name;
	password_ = password;
	max_member_ = 100;
	invOnly_ = 0;
	members_.push_back(*user);
	operators_.push_back(*user);
}

void Chanel::addOper(User *oper, User *user) {
	std::cout << "add Operator: " << std::endl;
	if (user == NULL) {
		for (auto&& memb : members_) {
			if (oper->getId() != memb.getId()) {
				user = &memb;
				break;
			}
		}
	}
	sendAll(oper, "MODE " + name_ + " +o " + user->getNick(), "", "");
	if (isOperators(user)) {
		return;
	}
	operators_.push_back(*user);
}

void Chanel::sendAll(User *usr, std::string arg1, std::string arg2, std::string arg3) {
	for (auto&& memb : members_) {
		Server::compileMsg(*usr, memb, arg1, arg2, arg3);
	}
}

std::string Chanel::getOperNames() {
	std::string ret = {};
	for (auto&& curr_oper : operators_) {
		ret += "@";
		ret += curr_oper.getNick() + " ";
	}
	ret.pop_back();
	return ret;
}

std::string Chanel::getUserNames() {
	std::string ret = {};
	for (auto&& curr_memb : members_) {
		if (operators_.end() != (std::find_if(operators_.begin(), operators_.end(), [&curr_memb](User& curr_op){ return curr_memb.getNick() == curr_op.getNick();}))) {
			ret += curr_memb.getNick() + " ";
		}
	}
	ret.pop_back();
	return ret;
}

void Chanel::addInvitedUser(User *user) {
	if (invited_.end() != std::find_if(invited_.begin(), invited_.end(), [&user](User& curr_inv){return user->getNick() == curr_inv.getNick();})) {
		return;
	}
	invited_.push_back(*user);
}