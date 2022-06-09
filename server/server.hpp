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

	/* 	Connection info */

		int					sock_fd;
		std::string			addr;
		std::string			port;
		struct sockaddr_in	info;

	/* irc info */
	
		bool	registred_connection;

	/*member functions*/
		client_t(void);
		bool operator==(client_t &rhs) const;
		bool operator==(const client_t &rhs) const;
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


/*
** Interaction
*/

bool client_is_in_server(const server_t &server, const client_t &client);
bool client_is_registred(const server_t &server, const client_t &client);

#endif
