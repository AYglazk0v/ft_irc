#include "../includes/Chanel.hpp"
#include "../includes/Server.hpp"

void Chanel::addOper(User *oper, User *user) {
	std::cout << "add Operator: " << std::endl;
	if (user == NULL) {
		for (auto&& memb : members_) {
			if (oper->getId() != memb.getId()) {
			// if (operators_[0].getId() != memb.getId()) { Bug???
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