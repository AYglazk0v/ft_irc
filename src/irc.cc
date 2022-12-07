#include "../includes/Server.hpp"

int main(int argc, char** argv) {
	std::cout << "\033[H\033[2J\033[3J" ;
	if (argc != 3) {
		error("Error: invalid arguments. You must specify the port and password. \n[example: ./irc 6654 password]");
	}
	Server srv(argv);
}