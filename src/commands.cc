#include "../includes/Commands.hpp"
#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

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

void Commands::cmd_motd(User *&user) {
    std::ifstream file("motd/daily.msg");
    std::string msg;
    if (file) {
        std::vector<std::string> vec = {"IRS_SERVER"};
        msg = compileReply(375, *user, vec);
        Server::sendMsg(user->getSockFd(), msg);
        while (std::getline(file, vec[0])) {
            msg = compileReply(372, *user, vec);
            Server::sendMsg(user->getSockFd(), msg);
        }
        msg = compileReply(376, *user, vec);
        Server::sendMsg(user->getSockFd(), msg);
    } else {
        msg = compileError(422, *user, "", "");
        Server::sendMsg(user->getSockFd(), msg);
    }
}