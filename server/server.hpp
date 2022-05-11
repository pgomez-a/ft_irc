#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>

struct client_t
{
	public:
		int			sock_fd;
		std::string		addr;
		std::string		port;
		struct sockaddr_in	info;
};

struct server_t
{
	public:
		int			sock_fd;
		int			timeout;
		int			clients_nfds;
		std::string		addr;
		std::string		port;
		std::string		passwd;
		struct addrinfo		hints;
		struct addrinfo*	res;
		struct pollfd		clients_fds[200];
		client_t		clients_info[200];
};

#endif
