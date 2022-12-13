#include "../includes/utils.hpp"
#include "..//includes/User.hpp"

void error(const std::string& msg) {
		std::cerr << msg << std::endl;
		exit(EXIT_FAILURE);
}

std::vector<std::string> split(std::string msg, char sym) {
	std::vector<std::string> ret;
	auto i = msg.begin();
	auto j = msg.begin();
	while (i != msg.end()) {
		i = std::find_if(i, msg.end(), [sym](char c){ return sym != c; });
		j = std::find_if(i, msg.end(), [sym](char c){ return sym == c; });
		if (i != msg.end()) {
			ret.push_back(std::string(i, j));
			i = j;
		}
	}
	return ret;
}

std::vector<std::string> splitMessage(std::string msg) {
	std::vector<std::string> ret;
	auto new_end = std::unique(msg.begin(), msg.end(), [](const char& c1, const char& c2){ return c1 == c2; });
	msg.erase(new_end, msg.end());
	if (msg.size() && msg[0] == ' ') {
		msg.erase(msg.begin());
	}
	if (msg.size() - 1 > 0 && msg[msg.size() - 1] == ' ') {
		msg.erase(msg.end() - 1);
	}
	auto i = msg.find(' ');
	auto j = msg.find(':');
	while (i != std::string::npos && i < j) {
		ret.push_back(msg.substr(0, i));
		msg.erase(0, i + 1);
		i = msg.find(' ');
		j = msg.find(':');
	}
	ret.push_back(msg);
	for (auto&& curr_ret : ret) {
		if (curr_ret.front() == ':') {
			curr_ret.erase(0, 1);
		}
	}
	return ret;
}

std::string CompileError(int index, User &usr, std::string arg1, std::string arg2) {
	std::string msg = ":IRC_SERVER " + std::to_string(index) + " " + usr.getNick() + " ";
	switch (index)
	{
		case 401:
			msg += arg1 + " :No such nick/channel\n";
			break;
		case 402:
			msg += arg1 + " : No such server\n";
			break;
		case 403:
			msg += arg1 + " :No such channel\n";
			break;
		case 404:
			msg += arg1 + " :Cannot send to channel\n"; 
			break;
		case 405:
			msg += arg1 + " :You have joined too many channels\n";
			break;
		case 406:
		 	msg += arg1 + " :There was no such nickname\n";
			break;
		case 407:
			msg += arg1 + " :Duplicate recipients. No arg1 delivered\n";
			break;
		case 409:
			msg += ":No origin specified\n";
			break;
		case 411:
			msg += ":No recipient given (" + arg1 + ")\n";
			break;
		case 412:
			msg += ":No text to send\n";
			break;
		case 413:
			msg += arg1 + " :No toplevel domain specified\n";
			break;
		case 414:
			msg += arg1 + " :Wildcard in toplevel domain\n";
			break;
		case 421:
			msg += arg1 + " :Unknown command\n";
			break;
		case 422:
			msg += ":MOTD File is missing\n";
			break;
		case 423:
			msg += arg1 + " :No administrative info available\n";
			break;
		case 424:
			msg += ":File error doing \n" + arg1 + " on " + arg2 + "\n";
			break;
		case 431:
			msg += ":No nickname given\n";
			break;
		case 432:
			msg += arg1 + ":Erroneus nickname\n";
			break;
		case 433:
			msg += arg1 + ":Nickname is already in use\n";
			break;
		case 436:
			msg += arg1 + " :Nickname collision KILL\n";
			break;
		case 441:
			msg += arg1 + " " + arg2 + " :They aren't on that channel\n";
			break;
		case 442:
			msg += arg1 + " :You're not on that channel\n";
			break;
		case 443:
			msg += arg1 + " " + arg2 + " :is already on channel\n";
			break;
		case 444:
			msg += arg1 + " :User not logged in\n";
			break;
		case 445:
			msg += ":SUMMON has been disabled\n";
			break;
		case 446:
			msg += ":USERS has been disabled\n";
			break;
		case 451:
			msg += ":You have not registered\n";
			break;
		case 461:
			msg += arg1 + ":Not enough parameters\n";
			break;
		case 462:
			msg += ":You may not reregister\n";
			break;
		case 463:
			msg += ":Your host isn't among the privileged\n";
			break;
		case 464:
			msg += ":Password incorrect\n";
			break;
		case 465:
			msg += ":You are banned from this server\n";
			break;
		case 467:
			msg += arg1 + " :Channel key already set\n";
			break;
		case 471:
			msg += arg1 + " :Cannot join channel (+l)\n";
			break;
		case 472:
			msg += arg1 + " :is unknown mode char to me\n";
			break;
		case 473:
			msg += arg1 + " :Cannot join channel (+i)\n";
			break;
		case 474:
			msg += arg1 + " :Cannot join channel (+b)\n";
			break;
		case 475:
			msg += arg1 + " :Cannot join channel (+k)\n";
			break;
		case 481:
			msg += ":Permission Denied- You're not an IRC operator\n";
			break;
		case 482:
			msg += arg1 + " :You're not channel operator\n";
			break;
		case 483:
			msg += ":You cant kill a server!\n";
			break;
		case 491:
			msg += ":No O-lines for your host\n";
			break;
		case 501:
			msg += ":Unknown MODE flag\n";
			break;
		case 502:
			msg += ":Cant change mode for other users\n";
			break;
		default:
			msg += "UNKNOWN ERROR\n";
			break;
	}
	return  msg;
}

std::string compileReply(int index, User &usr, std::vector<std::string>& args) {
	std::string msg = ":IRC_SERVER " + std::to_string(index) + " " + usr.getNick() + " ";
	switch (index) {
		case 200:
			msg += "Link " + args[0] + " " + args[1] + " " + args[2];
			break;
		case 201:
			msg += "Try. " + args[0] + " " + args[1];
			break;
		case 202:
			msg += "H.S. " + args[0] + " " + args[1];
			break;
		case 203:
			msg += "????" + args[0] + " " + args[1];
			break;
		case 204:
			msg += "Oper " + args[0] + " " + args[1];
			break;
		case 205:
			msg += "User " + args[0] + " " + args[1];
			break;
		case 206:
			msg += "Serv " + args[0] + " " + args[1] + "S " + args[2] + "C " + args[3] + " " + args[4] + "@" + args[5];
			break;
		case 208:
			msg += args[0] + " O " + args[1];
			break;

		case 211:
			msg += args[0] + " " + args[1] + " " + args[2] + " " + args[3] + " " + args[4] + " " + args[5] + " " + args[6];
			break;
		case 212:
			msg += args[0] + " "+ args[1];
			break;
		case 213:
			msg += "C " + args[0] + " * " + args[1] + " " + args[2] + " " + args[3];
			break;
		case 214:
			msg += "N " + args[0] + " * " + args[1] + " " + args[2] + " " + args[3];
			break;
		case 215:
			msg += "I " + args[0] + " * " + args[1] + " " + args[2] + " " + args[3];
			break;
		case 216:
			msg += "K " + args[0] + " * " + args[1] + " " + args[2] + " " + args[3];
			break;
		case 218:
			msg += "Y " + args[0] + " * " + args[1] + " " + args[2] + " " + args[3];
			break;
		case 219:
			msg += args[0] + " :End of /STATS report";
			break;
		case 221:
			msg += args[0];
			break;
		case 241:
			msg += "L " + args[0] + " * " + args[1] + " " + args[2];
			break;
		case 242:
			msg += ":Server Up %d days %d:%02d:%02d";
			break;
		case 243:
			msg += "O " + args[0] + " * " + args[1];
			break;
		case 244:
			msg += "H " + args[0] + " * " + args[1];
			break;

		case 251:
			msg += ":There are " + args[0] + " users and " + args[1] + " invisible on " + args[2] + " servers";
			break;
		case 252:
			msg += args[0] + " :operator(s) online";
			break;
		case 253:
			msg += args[0] + " unknown connection(s)";
			break;
		case 254:
			msg += args[0] + " :channels formed";
			break;
		case 255:
			msg += "I have " + args[0] + " clients and " + args[1] + " servers";
			break;
		case 256:
			msg += args[0] + " :Administrative info";
			break;
		case 257:
			msg += ":Name " + args[0];
			break;
		case 258:
			msg += ":Nickname " + args[0];
			break;
		case 259:
			msg += ":E-Mail " + args[0];
			break;
		case 261:
			msg += "File " + args[0] + " " + args[1];
			break;
		case 301:
			msg += args[0] + " :" + args[1];
			break;
		case 302:
			msg += ":" + args[0];	
			break;
		case 303:
			msg += ":" + args[0];
			break;
		case 305:
			msg += "You are no loger marked as being away";
			break;
		case 306:
			msg += "You have been marked as being away";
			break;
		case 311:
			msg += args[0] + " " + args[1] + " " + args[2] + " * :" + args[3];
			break;
		case 312:
			msg += args[0] + " " + args[1] + " :" + args[2];
			break;
		case 313:
			msg += args[0] + " :is an IRC operator";
			break;
		case 314:
			msg += args[0] + " " + args[1] + " " + args[2] + " * :" + args[3];
			break;
		case 315:
			msg += args[0] + " :End of /WHO list";
			break;
		case 317:
			msg += args[0] + " " + args[1] + " " + args[2] + " :seconds idle";
			break;
		case 318:
			msg += args[0] + " :End of /WHOIS list";		
			break;
		case 319:
			msg += args[0] + " :" + args[1];
			break;


		case 321:
			msg += "Channel :Users Name";
			break;
		case 322:
			msg += args[0] + " " + args[1] + " :" + args[2];
			break;
		case 323:
			msg += "END of /LIST";
			break;
		case 324:
			msg += args[0] + " +" + args[1];
			break;
		case 331:
			msg += args[0] + " :No topic is set";
			break;
		case 332:
			msg += args[0] + " :" + args[1];
			break;
		case 341:
			msg += args[0] + " " + args[1];
			break;
		case 342:
			msg += args[0] + " :Summoning user to IRC";
			break;
		case 351:
			msg += args[0] + "." + args[1] + " " + args[2] + " :" + args[3];
			break;
		case 352:
			msg += args[0] + " " + args[1] + " " + args[2] + " " + args[3] + " " + args[4] + " " + args[5] + " :" + args[6] + args[7];
			break;

		case 353:
			msg += args[0] + " :" + args[1];
			break;

		case 364:
			msg += args[0] + " " + args[1] + ": " + args[2] + " " + args[3];
			break;
		case 365:
			msg += args[0] + " :End of /LINKS list";
			break;
		case 366:
			msg += args[0] + " :End of /NAMES list";
			break;
		case 367:
			msg += args[0] + " " + args[1];
			break;
		case 368:
			msg += args[0] + " :End of channel ban list";
			break;
		case 369:
			msg += args[0] + " :End of WHOWAS";
			break;
		case 371:
			msg += ":" + args[0];
			break;
		case 372:
			msg += ":- " + args[0];
			break;
		case 374:
			msg += ":End of /INFO list";
			break;
		case 375:
			msg += ":- " + args[0] + " Message of the day - ";
			break;

		case 376:
			msg += ":End of /MOTD command";
			break;
		case 381:
			msg += ":You are now an IRC operator";
			break;
		case 382:
			msg += args[0] + " :Rehashing";
			break;
		case 391:
			msg += args[0] + " :" + args[1];
			break;
		case 392:
			msg += ":UserID Terminal Host";
			break;
		case 393:
			msg += ":%-8s %-9s %-8s";
			break;
		case 394:
			msg += "End of users";
			break;
		case 395:
			msg += "Nobody logged is";
			break;

		default:
			msg += "UNKNOWN REPLY";
			break;
	}
	msg += "\n";
	return msg;
}