#include "server.hpp"

client_t	*server_t::find(std::string nick)
{
	for (int i = 0; i < clients_nfds; ++i)
	{
		if (clients_info[i].get_nick() == nick)
			return &clients_info[i];
	}
	return NULL;

}
