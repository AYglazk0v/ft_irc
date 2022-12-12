#ifndef COMANDS_HPP
# define COMANDS_HPP

# include "utils.hpp"
# include "User.hpp"
# include "Server.hpp"

class Commands {
	public:
		static int	cmd_pass(std::vector<std::string> args, User*& user, Server* data);
		static int	cmd_user(std::vector<std::string> args, User& user);
		static int	cmd_ping(std::vector<std::string> args, User& user);
		static int	cmd_pong(std::vector<std::string> args, User& user);
		static void	cmd_nick(std::vector<std::string> args, User& user, std::vector<User *>& users);
		static void	cmd_motd(User& user);
		static void	cmd_privmsg(std::vector<std::string> args, User& user, std::vector<User *>& users, std::vector<Chanel *>& chanels, bool notice);
		static void	cmd_away(std::vector<std::string> args, User& user);
		static void	cmd_ison(std::vector<std::string> args, User& user, std::vector<User*>& users);
		static void cmd_admin(std::vector<std::string> args, User& user);
		static void cmd_info(std::vector<std::string> args, User& user);
		static void cmd_join(std::vector<std::string> args, User& user, std::vector<User* >& users,std::vector<Chanel *>& chanels);
		static void cmd_invite(std::vector<std::string> args, User& user, std::vector<User* >& users,std::vector<Chanel *>& chanels);
		static void cmd_who(std::vector<std::string> args, User& user, std::vector<User* >& users,std::vector<Chanel *>& chanels);
		static std::string Chanel_who(User& user, std::vector<Chanel *>& chanels);
		static void cmd_part(std::vector<std::string> args, User& user, std::vector<User *>& users, std::vector<Chanel *>& chanels);
		static void cmd_whois(std::vector<std::string> args, User& user, std::vector<User *>& users, std::vector<Chanel *>& chanels);
		static void	cmd_topic(std::vector<std::string> args, User& user, std::vector<User *>& users, std::vector<Chanel *>& chanels);
		static void cmd_kick(std::vector<std::string> args, User& user, std::vector<User *>& users, std::vector<Chanel *>& chanels);
		static void cmd_mode(std::vector<std::string> args, User& user, std::vector<User *>& users, std::vector<Chanel *>& chanels);
};
#endif