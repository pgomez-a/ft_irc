#include "ircserv.hpp"

/**
 ** getaddrinfo - socket address structure to host and service name
 **
 ** int getaddrinfo(const char *hostname,
 **                 const char *servname,
 **                 const struct addrinfo *hints,
 **                 struct addrinfo **res);
 **/

int	init_addrinfo(char* port, struct addrinfo& hints, struct addrinfo*& res)
{
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_DEFAULT;
	if (getaddrinfo("localhost", port, &hints, &res) != 0)
	{
		std::cerr << "\033[1m\033[91mError:\033[0m\033[91m getaddrinfo()\n\033[0m";
		return (-1);
	}
	return (0);
}

/**
 ** socket     - create an endpoint for communication
 ** setsockopt - get and set options on sockets
 ** fcntl      – file control
 **
 ** int socket(int domain,     int setsockopt(int socket, int level,     int fcntl(int fildes,
 **            int type,                      int option_name,                     int cmd,
 **            int protocol);                 const void *option_value,            ...);
 **                                           socklen_t option_len);
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
 ** bind – bind a name to a socket
 **
 ** int bind(int socket,
 **          const struct sockaddr *address,
 **          socklen_t address_len);
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
 ** listen – listen for connections on a socket
 **
 ** int listen(int socket,
 **            int backlog);
 **/

int	listen_socket(int sock_fd, int capacity, char* port)
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
	std::cout << "\n                         Hosted at 127.0.0.1:" << port << std::endl;
	std::cout << "\033[0m\n\n";
	return (0);
}
