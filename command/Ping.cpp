#include "Ping.hpp"

Ping::Ping(void)
{
	_command_name = "PING";
	_id = PING;
}

int	Ping::_effect(server_t &server, client_t &client)
{
	std::string s = "PONG " + server.name + " " + client.addr + "\r\n";
	if (!_argc)
		return ERR_NOORIGIN;
	// else if no such server, return R_ERR_NOSUCHSERVER
	send_to_client(s, client);
	return 0;
}
