#ifndef CHANEL_HPP
# define CHANEL_HPP

#include "User.hpp"

class User;

class Chanel {
	private:
		std::string			name_;
		std::string			password_;
		size_t				max_member_;
		bool				invOnly_;
		std::vector<User>	members_;
		std::vector<User>	operators_;
		std::vector<User>	invited_;
		std::vector<User>	invisibles_;
		std::vector<User>	banned_;
		std::string			topic_;
	public:
		Chanel(User *user, std::string name, std::string password);
		
		void addOper(User* oper, User* user);
		void addInvitedUser(User* user);
		void sendAll(User* usr, std::string arg1, std::string arg2, std::string arg3);
		
		void addUser(User* user)			{ members_.push_back(*user); }
		void setTopic(std::string topic)	{ topic_ = topic; }

		std::string getName() const		{ return name_; }
		std::string getPassword() const	{ return password_; }
		std::string getTopic() const	{ return topic_;}

		size_t memberSize() const		{ return members_.size(); }
		size_t operSize() const			{ return operators_.size(); }
		size_t max_member() const		{ return max_member_; }
		
		bool getInvOnly() const			{return invOnly_;}

		std::string	getOperNames();
		std::string getUserNames();

		bool isMember(User *usr) const		{ return (members_.end() != std::find_if(members_.begin(), members_.end(), [&usr](const User& curr_cmp) { return usr->getId() == curr_cmp.getId(); })); }
		bool isOperators(User *usr) const	{ return (operators_.end() != std::find_if(operators_.begin(), operators_.end(), [&usr](const User& curr_cmp) { return usr->getId() == curr_cmp.getId(); })); }
		bool isInvited(User *usr) const		{ return (invited_.end() != std::find_if(invited_.begin(), invited_.end(), [&usr](const User& curr_cmp) { return usr->getId() == curr_cmp.getId(); })); }
		bool isInvisble(User *usr) const	{ return (invisibles_.end() != std::find_if(invisibles_.begin(), invisibles_.end(), [&usr](const User& curr_cmp) { return usr->getId() == curr_cmp.getId(); })); }
		bool isBanned(User *usr) const		{ return (banned_.end() != std::find_if(banned_.begin(), members_.end(), [&usr](const User& curr_cmp) { return usr->getId() == curr_cmp.getId(); })); }

		void removeMember(User *usr)		{ members_.erase(std::remove_if(members_.begin(), members_.end(), [&usr](User& curr){return usr->getId() == curr.getId(); }));}
		void removeOperator(User *usr)		{ operators_.erase(std::remove_if(operators_.begin(), operators_.end(), [&usr](User *curr){ return curr->getId() == usr->getId(); }));}

}; //Chanel

#endif