#ifndef USER_HPP
# define USER_HPP

// # include "Server.hpp"
# include "utils.hpp"
# include <regex>
class User {
	private:
		int					id_;
		int 				sock_fd_;
		struct sockaddr_in	addres_;

		bool				access_;
		bool				autorization_;
		bool				away_;
		bool				oper_;
		bool				invis_;
		
		std::string			nick_;
		std::string			user_;
		std::string			host_;
		std::string			serverName_;
		std::string			realName_;
		std::string			buff_;
		std::string			away_msg_;

	public:
		User(int id, int sock_fd, sockaddr_in addres) : id_(id), sock_fd_(sock_fd), addres_(addres) {
			access_ = 0;
			autorization_ = 0;
			away_ = 0;
			oper_ = 0;
			invis_ = 0;
		}

		void clearBuff(){
			buff_ = "";
			buff_.clear();
		}

		bool createBuff(int read_b, char *buff){
			buff[read_b] = '\0';
			if (buff_.empty()) {
				buff_ = buff;
			} else {
				buff_ += buff;
			}
			if (buff[read_b - 1] != '\n') {
				return false;
			}
			std::regex rx("\r\n");
			buff_ = std::regex_replace(buff_, rx, "\n");
			return true;
		}

		int getId() const					{ return id_; }
		int getSockFd()	const				{ return sock_fd_; }
		sockaddr_in getAddr()				{ return addres_;}
		bool getAccess() const				{ return access_; }
		bool getAutorization() const		{ return autorization_; }
		bool getAway() const				{ return away_; }
		bool getOper() const				{ return oper_; }
		bool getInvis()	const				{ return invis_; }
		std::string getNick() const			{ return nick_; }
		std::string getUsername() const			{ return user_; }
		std::string getHost() const			{ return host_; }
		std::string getServerName() const	{ return serverName_; }
		std::string	getRealName() const 	{ return realName_; }
		std::string getBuff() const			{ return buff_; }
		std::string	getAwayMsg() const		{ return away_msg_; }

		void setId(int id) 							{ id_ = id; }
		void setSockFd(int sock_fd)					{ sock_fd_ = sock_fd;}
		void setAddr(sockaddr_in addr)				{ addres_ = addr; }
		void setAccess(bool acc)					{ access_ = acc; }
		void setAutorizatuion(bool aut)				{ autorization_ = aut; }
		void setAway(bool away)						{ away_ = away; }
		void setOper(bool op)						{ oper_ = op; }
		void setInvis(bool invis)					{ invis_ = invis; }
		void setNick(std::string nick)				{ nick_ = nick; }
		void setUser(std::string user)				{ user_ = user; }
		void setHost(std::string host)				{ host_ = host; }
		void setServerName(std::string servName)	{ serverName_ = servName; }
		void setRealName(std::string realName)		{ realName_ = realName; }
		void setAwayMsg(std::string msg)			{ away_msg_ = msg; }


};//User

#endif