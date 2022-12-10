#ifndef CHANEL_HPP
# define CHANEL_HPP

#include "User.hpp"
#include <algorithm>
#include <memory>
#include <sys/types.h>

class User;

#include <string>
#include <vector>
class Chanel {
	private:
		std::string			name_;
		std::string			password_;
		int					max_member_;
		bool				invOnly_;
		std::vector<User>	members_;
		std::vector<User>	operators_;
		std::vector<User>	invited_;
		std::vector<User>	invisibles_;
		std::vector<User>	banned_;
		std::string			topic_;
	public:
		Chanel(User *user, std::string name, std::string password);
		
		std::string getName() const		{ return name_; }
		std::string getPassword() const	{ return password_; }
		std::string getTopic() const	{ return topic_;}

		bool isMember(User *usr) const { return (members_.end() != std::find_if(members_.begin(), members_.end(), [&usr](const User& curr_cmp) { return usr->getId() == curr_cmp.getId(); })); }
		bool isOperators(User *usr) const { return (operators_.end() != std::find_if(operators_.begin(), operators_.end(), [&usr](const User& curr_cmp) { return usr->getId() == curr_cmp.getId(); })); }
		bool isInvited(User *usr) const { return (invited_.end() != std::find_if(invited_.begin(), invited_.end(), [&usr](const User& curr_cmp) { return usr->getId() == curr_cmp.getId(); })); }
		bool isInvisble(User *usr) const { return (invisibles_.end() != std::find_if(invisibles_.begin(), invisibles_.end(), [&usr](const User& curr_cmp) { return usr->getId() == curr_cmp.getId(); })); }
		bool isBanned(User *usr) const { return (banned_.end() != std::find_if(banned_.begin(), members_.end(), [&usr](const User& curr_cmp) { return usr->getId() == curr_cmp.getId(); })); }

}; //Chanel

#endif