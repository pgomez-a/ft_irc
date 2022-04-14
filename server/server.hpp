#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sstream>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <fcntl.h>
# include <poll.h>
# include "./../lexer/lexer.hpp"

/**
 ** config_socket.cpp
 **/

int	init_addrinfo(char* port, struct addrinfo& hints, struct addrinfo*& res);

int	init_socket(struct addrinfo*& res);

int	bind_socket(int sock_fd, struct addrinfo*& res);

int	listen_socket(int sock_fd, int capacity);

/**
 ** manage_socket.cpp
 **/

int	manage_socket(int sock_fd, struct sockaddr_in* res);

#endif
