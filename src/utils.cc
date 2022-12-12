#include "../includes/utils.hpp"
#include "..//includes/User.hpp"
#include <string>

void error(const std::string& msg) {
		std::cerr << msg << std::endl;
		exit(EXIT_FAILURE);
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