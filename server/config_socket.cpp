#include "ircserv.hpp"

/**
 ** Inits addrinfo struct before socket configuration.
 **  - Communication Protocol: IPv4 (AF_INET)
 **  - Socket Type: SOCK_SREAM (TCP)
 **  - IP Addr: localhost (127.0.0.1)
 **/

int	init_addrinfo(char* port, struct addrinfo& hints, struct addrinfo*& res)
{
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, port, &hints, &res) != 0)
	{
		std::cerr << "\033[1m\033[91mError:\033[0m\033[91m getaddrinfo()\n\033[0m";
		return (-1);
	}
	return (0);
}

/**
 ** Sets socket configuration.
 **  - Communication Protocol: ai_family (IPv4)
 **  - Socket Type: ai_socktype (TCP)
 **  - Transport Protocol: ai_protocol (TCP)
 **
 ** The socket is set as non-blocking and its addr:port can be reused.
 **/

int	init_socket(struct addrinfo*& res)
{
	int	sock_fd;
	int	sock_op;

	sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock_fd == -1)
	{
		std::cerr << "\033[1m\033[91mError:\033[0m\033[91m socket()\n\033[0m";
		return (-1);
	}
	sock_op = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &sock_op, sizeof(sock_op)) == -1)
	{
		std::cerr << "\033[1m\033[91mError:\033[0m\033[91m setsockopt()\n\033[0m";
		close(sock_fd);
		return (-1);
	}
	if (fcntl(sock_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "\033[1m\033[91mError:\033[0m\033[91m fcntl()\n\033[0m";
		close(sock_fd);
		return (-1);
	}
	return (sock_fd);
}

/**
 ** Assigns the socket the set address and port.
 **/

int	bind_socket(int sock_fd, struct addrinfo*& res)
{
	if (bind(sock_fd, res->ai_addr, res->ai_addrlen) == -1)
	{
		std::cerr << "\033[1m\033[91mError:\033[0m\033[91m bind()\n\033[0m";
		close(sock_fd);
		return (-1);
	}
	return (0);
}

/**
 ** Sets the socket on listener mode.
 **/

int	listen_socket(int sock_fd, int capacity)
{
	if (listen(sock_fd, capacity) == -1)
	{
		std::cerr << "\033[1m\033[91mError:\033[0m\033[91m listen()\n\033[0m";
		close(sock_fd);
		return (-1);
	}
	std::cout << "\033[1m\n";
	std::cout << "   __  __                               ___\n";
	std::cout << "  /\\ \\/\\ \\                             /\\_ \\\n";
	std::cout << "  \\ \\ `\\\\ \\     __      ___ ___      __\\//\\ \\      __    ____    ____\n";
	std::cout << "   \\ \\ ,   \\  /'__ \\  /' __  __ \\  /'__ \\\\ \\ \\   /'__ \\ /',__\\  /',__\\\n";
	std::cout << "    \\ \\ \\`\\ \\/\\ \\L\\.\\_/\\ \\/\\ \\/\\ \\/\\  __/ \\_\\ \\_/\\  __//\\__,  \\/\\__,  \\\n";
	std::cout << "     \\ \\_\\ \\_\\ \\__/.\\_\\ \\_\\ \\_\\ \\_\\ \\____\\/\\____\\ \\____\\/\\____/\\/\\____/\n";
	std::cout << "      \\/_/\\/_/\\/__/\\/_/\\/_/\\/_/\\/_/\\/____/\\/____/\\/____/\\/___/  \\/___/\n";
	std::cout << "\033[0m\n\n";
	return (0);
}
