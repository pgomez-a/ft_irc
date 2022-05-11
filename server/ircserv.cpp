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
	int			sock_fd;
	struct addrinfo		hints;
	struct addrinfo*	res = 0;

	//atexit(leaks);
	if (argc != 2)
	{
		std::cerr << "\033[1m\033[91mError:\033[0m\033[91m ./ircserv port\n\033[0m";
		return (1);
	}
	if (init_addrinfo(argv[1], hints, res) == -1)
		return (1);
	sock_fd = init_socket(res);
	if (sock_fd == -1)
		return (1);
	if (bind_socket(sock_fd, res) == -1)
		return (1);
	if (listen_socket(sock_fd, 10, argv[1]) == -1)
		return (1);
	manage_socket(sock_fd, (struct sockaddr_in*)res->ai_addr);
	freeaddrinfo(res);
	close(sock_fd);
	return (0);
}
