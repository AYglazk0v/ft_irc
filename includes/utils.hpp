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

#endif