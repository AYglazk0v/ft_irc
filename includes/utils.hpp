#ifndef UTILS_HPP
# define UTILS_HPP

// #include "User.hpp"
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

void        error(const std::string& msg);
std::vector<std::string> split(std::string msg, char sym);
std::vector<std::string> splitMessage(std::string msg);
#endif