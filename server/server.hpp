#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <fcntl.h>
# include <poll.h>

struct client_t
{
	public:
		int					sock_fd;
		std::string			addr;
		std::string			port;
		struct sockaddr_in	info;
};

struct server_t
{
	public:
		int					sock_fd;
		int					timeout;
		int					clients_nfds;
		std::string			name;
		std::string			addr;
		std::string			port;
		std::string			passwd;
		struct addrinfo		hints;
		struct addrinfo*	res;
		struct pollfd		clients_fds[1024];
		client_t			clients_info[1024];
};

#endif
