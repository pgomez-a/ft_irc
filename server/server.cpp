#include "server.hpp"

client_t	*server_t::find(std::string nick)
{
	//fix sent reference...
	client_t *c = new client_t;
	for (size_t i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clients_info[i].get_nick() == nick)
			return c;
	}
	return NULL;

}
