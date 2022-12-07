#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <cstring>
# include <algorithm>
# include <cctype>

void error(const std::string& msg) {
		std::cerr << msg << std::endl;
		exit(EXIT_FAILURE);
}

#endif