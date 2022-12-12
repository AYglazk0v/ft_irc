#include "../includes/Commands.hpp"

int Commands::cmd_pass(std::vector<std::string> args, User &user, Server *data) {
    std::string msg;
    if (args.size() < 2) {
        msg = CompileError(411, user, args[0], "");
        Server::sendMsg(user.getSockFd(), msg);
        return 0;
    }
    return 1;;
}