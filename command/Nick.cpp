#include "Nick.hpp"

Nick::Nick(void) {
	_command_name = "NICK";
	_id = NICK;
};

std::string	Nick::_effect(server_t &server, client_t &client)
{
	
	return reply_format(server.name, "999", client.addr, "NICK's EXCLUSIVE EFFECT");
}