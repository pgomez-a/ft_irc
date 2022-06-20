#include "Ping.hpp"

Ping::Ping(void)
{
	_command_name = "PING";
	_id = PING;
}

int	Ping::_effect(server_t &server, client_t &client)
{
	std::string s;
	if (!_argc)
		return ERR_NOORIGIN;
	// else if no such server, return R_ERR_NOSUCHSERVER; but we don't have server intercomm
	s = "PONG " + server.name + " " +  _argt[0] + "\r\n";
	send_to_client(s, client);
	return 0;
}
