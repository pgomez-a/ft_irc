#include "server.hpp"

client_t	*server_t::find_nick(std::string nick)
{
	for (int i = 0; i < clients_nfds; ++i)
	{
		if (clients_info[i].get_nick() == nick)
			return &clients_info[i];
	}
	return NULL;
}

client_t	*server_t::find_user(std::string user)
{
	for (int i = 0; i < clients_nfds; ++i)
	{
		if (clients_info[i].get_user() == user)
			return &clients_info[i];
	}
	return NULL;
}

bool	server_t::valid_oper_host(client_t &client)
{
	for (std::list<std::string>::iterator i = no_oper_list.begin(); i != no_oper_list.end(); ++i)
	{
		if (client.addr == *i)
			return false;
	}
	return true;
}
