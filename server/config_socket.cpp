#include "ircserv.hpp"

/**
 ** Inits addrinfo struct before socket configuration.
 **  - Communication Protocol: IPv4 (AF_INET)
 **  - Socket Type: TCP (SOCK_SREAM)
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
		put_error("getaddrinfo()");
		return -1;
	}
	return 0;
}

/**
 ** Sets socket configuration.
 **  - Communication Protocol: IPv4 (ai_family)
 **  - Socket Type: TCP (ai_socktype)
 **  - Transport Protocol: TCP (ai_protocol)
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
		put_error("socket()");
		return -1;
	}
	sock_op = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &sock_op, sizeof(sock_op)) == -1)
	{
		put_error("setsockopt()");
		close(sock_fd);
		return -1;
	}
	if (fcntl(sock_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		put_error("fcntl()");
		close(sock_fd);
		return -1;
	}
	return sock_fd;
}

/**
 ** Assigns the socket the set address and port.
 **/

int	bind_socket(int sock_fd, struct addrinfo*& res)
{
	if (bind(sock_fd, res->ai_addr, res->ai_addrlen) == -1)
	{
		put_error("bind()");
		close(sock_fd);
		return -1;
	}
	return 0;
}

/**
 ** Sets the socket on listener mode.
 **/

int	listen_socket(int sock_fd, int capacity)
{
	if (listen(sock_fd, capacity) == -1)
	{
		put_error("listen()");
		close(sock_fd);
		return -1;
	}
	put_event("   __  __                               ___", BOLD);
	put_event("  /\\ \\/\\ \\                             /\\_ \\", BOLD);
	put_event("  \\ \\ `\\\\ \\     __      ___ ___      __\\//\\ \\      __    ____    ____", BOLD);
	put_event("   \\ \\ ,   \\  /'__ \\  /' __  __ \\  /'__ \\\\ \\ \\   /'__ \\ /',__\\  /',__\\", BOLD);
	put_event("    \\ \\ \\`\\ \\/\\ \\L\\.\\_/\\ \\/\\ \\/\\ \\/\\  __/ \\_\\ \\_/\\  __//\\__,  \\/\\__,  \\", BOLD);
	put_event("     \\ \\_\\ \\_\\ \\__/.\\_\\ \\_\\ \\_\\ \\_\\ \\____\\/\\____\\ \\____\\/\\____/\\/\\____/", BOLD);
	put_event("      \\/_/\\/_/\\/__/\\/_/\\/_/\\/_/\\/_/\\/____/\\/____/\\/____/\\/___/  \\/___/", BOLD);
	put_event("");
	return 0;
}
