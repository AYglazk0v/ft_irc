#include "../includes/Commands.hpp"

int Commands::cmd_pass(std::vector<std::string> args, User* &user, Server *data) {
    std::string msg;
    if (args.size() < 2) {
        msg = compileError(411, *user, args[0], "");
        Server::sendMsg(user->getSockFd(), msg);
        return 0;
    }
    if (user->getAccess()) {
        msg = compileError(462, *user, "", "");
        data->sendMsg(user->getSockFd(), msg);
        return 0;
    }
    if (args[1] != data->getPassword()) {
        msg = compileError(464, *user, "", "");
        data->sendMsg(user->getSockFd(), msg);
        data->disconnect(user);
        return 1;
    } else {
        user->setAccess(1);
    }
    return 0;
}