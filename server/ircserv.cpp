#include "ircserv.hpp"

/**
 ** Makes the necessary configurations to activate the server: sockets, address, port, clients, etc...
 ** Creates the file that stores the connection history.
 **/

static int	init_server_configuration(int argc, char *argv[], server_t &server)
{
	std::ofstream	history;

	if (argc != 3)
	{
		put_error("./ircserv <port> <passwd>");
		return -1;
	}
	if (init_addrinfo(argv[1], server.hints, server.res) == -1)
		return -1;
	server.sock_fd = init_socket(server.res);
	if (server.sock_fd == -1)
		return -1;
	if (bind_socket(server.sock_fd, server.res) == -1)
		return -1;
	if (listen_socket(server.sock_fd, 10))
		return -1;
	history.open(".nameless_history", std::fstream::trunc);
	if (!history.is_open())
	{
		put_error("./ircserv ofstream.open()");
		close(server.sock_fd);
		return -1;
	}
	history.close();
	return 0;
}

/**
 ** Main entrance to the server.
 **/

int	main(int argc, char* argv[])
{
	server_t	server;

	if (init_server_configuration(argc, argv, server) == -1)
		return 1;
	init_server(argv[1], argv[2], server);
	manage_socket(server);
	freeaddrinfo(server.res);
	close(server.sock_fd);
	return (0);
}
