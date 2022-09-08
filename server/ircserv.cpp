#include "ircserv.hpp"

/**
 ** Makes the necessary configurations to activate the server: sockets, address, port, clients, etc...
 ** Creates the file that stores the connection history.
 **/

static int	init_server(int argc, char *argv[], server_t &server)
{
	std::ofstream	history;

	if (!(argc == 3 || argc == 4))
	{
		put_error("./ircserv [host:port_network:password_network] <port> <passwd>");
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
 ** Once the server is up, it is configured with the parameters entered by the user.
 **/

static int	config_server(char* port, char* passwd, server_t& server)
{
	server.port = static_cast<std::string>(port);
	server.passwd = static_cast<std::string>(passwd);
	server.oper_passwd = "oper"; //harcoded access to Operator flag //harcoded access to Operator flag
	//server.version = "0.1";
	server.timeout = 5 * 1000 * 60;
	server.clients_fds[0].fd = server.sock_fd;
	server.clients_fds[0].events = POLLIN;
	server.clients_nfds = 1;
	return (0);
}


/**
 ** Main entry to the server.
 **/

int			main(int argc, char* argv[])
{
	server_t	server;
	int			extra_arg;

	extra_arg = (argc == 4);
	if (init_server(argc, argv + extra_arg, server) == -1)
		return 1;
	config_server(argv[1 + extra_arg], argv[2 + extra_arg], server);
	manage_socket(server);
	freeaddrinfo(server.res);
	close(server.sock_fd);
	return 0;
}
