#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <cstring>
# include <algorithm>
# include <unistd.h>
# include <cctype>
# include <sys/select.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <vector>

class User;

std::string compileError(int index, User& usr, std::string arg1, std::string arg2);
std::string compileReply(int index, User& usr, std::vector<std::string>& args);

void        error(const std::string& msg);
std::vector<std::string> split(std::string msg, char sym);
std::vector<std::string> splitMessage(std::string msg);
void		*ping_request(void *req_res);

#endif