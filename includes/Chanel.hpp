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
		std::vector<User>	ivited_;
		std::vector<User>	invisables_;
		std::vector<User>	baned_;
		std::string			topic_;
	public:
		Chanel(User *user, std::string name, std::string password);
		
		std::string getName()		{ return name_; }
		std::string getPassword()	{ return password_; }
		std::string getTopic()		{ return topic_;}

		bool isMember(User *usr) const { return (members_.end() != std::find_if(members_.begin(), members_.end(), [&usr](const User& curr_cmp) { return usr->getId() == curr_cmp.getId(); })); }

}; //Chanel

#endif