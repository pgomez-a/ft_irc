#include "Nick.hpp"

Nick::Nick(void) {
	_command_name = "NICK";
	_id = NICK;
};

int	Nick::_effect(server_t &server, client_t &client)
{
	(void)server, (void)client;
	return 0;
}