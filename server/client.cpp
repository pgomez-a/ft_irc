#include "server.hpp"

bool client_is_in_server(const server_t &server, const client_t &client)
{
	//needs to be refactored, clients_info should be a map instead of an array, and client_t should have
	//comparison overload among other things
	for (size_t i = 0; i < 1024; ++i)
	{
		if (server.clients_info[i] == client)
			return true;
	}
	return false;
}

bool client_is_registred(const server_t &server, const client_t &client)
{
	return (client.registred_connection && client_is_in_server(server, client));
}

/* Default constructor */
client_t::client_t(void):sock_fd(), addr(), port(), info(), registred_connection(0){}

bool client_t::operator==(client_t &rhs) const
{
	return (this->port == rhs.port && this->addr == rhs.addr);
}

bool client_t::operator==(const client_t &rhs) const
{
	return (this->port == rhs.port && this->addr == rhs.addr);
}