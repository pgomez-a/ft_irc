#include "ircserv.hpp"

/**
static void	leaks(void)
{
	std::cout << "\n\n";
	system("leaks server");
	return ;
}
**/

int	main(int argc, char* argv[])
{
	server_t	server;

	//atexit(leaks);
	if (argc != 3)
	{
		std::cerr << "\033[1m\033[91mError:\033[0m\033[91m ./ircserv <port> <passwd>\n\033[0m";
		return (1);
	}
	if (init_addrinfo(argv[1], server.hints, server.res) == -1)
		return (1);
	server.sock_fd = init_socket(server.res);
	if (server.sock_fd == -1)
		return (1);
	if (bind_socket(server.sock_fd, server.res) == -1)
		return (1);
	if (listen_socket(server.sock_fd, 10, argv[1]) == -1)
		return (1);
	init_server(argv[1], argv[2], server);
	manage_socket(server);
	freeaddrinfo(server.res);
	close(server.sock_fd);
	return (0);
}
