#ifndef REPLY_SYSTEM_HPP
# define REPLY_SYSTEM_HPP
# include "server.hpp"
# include "output.hpp"
# include "sys/socket.h"

std::string		reply_format(std::string prefix, std::string command_code, std::string target, std::string message);
int				reply_to_client(client_t &client, std::string reply);

#endif