#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include <iostream>
# include <fstream>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <fcntl.h>
# include <poll.h>
# include "server.hpp"
# include "parser.hpp"

/** config_socket.cpp **/
int	init_addrinfo(char* port, struct addrinfo& hints, struct addrinfo*& res);

int	init_socket(struct addrinfo*& res);

int	bind_socket(int sock_fd, struct addrinfo*& res);

int	listen_socket(int sock_fd, int capacity);


/** manage_socket.cpp **/
int	manage_socket(server_t& server);


/** executor.cpp **/
int 	executor(char *buf, size_t buf_len, server_t &server, client_t &client);

#endif
