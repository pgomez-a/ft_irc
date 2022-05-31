#include "reply_system.hpp"

std::string		reply_format(std::string prefix, std::string command_code, std::string target, std::string message)
{
	std::string	s;

	s += ":" + prefix + " " + command_code + " " + target + " :" + message + "\r\n";

	return s;
}

int reply_to_client(client_t &client, std::string reply)
{
	int	r;

	r = send(client.sock_fd, reply.c_str(), reply.size(), 0);
	return r;
}