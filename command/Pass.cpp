#include "Pass.hpp"

Pass::Pass(void) {
	_command_name = "PASS";
	_id = PASS;
};

int	Pass::_effect(server_t &server, client_t &client)
{
	if (argc < 1)
		return ERR_NEEDMOREPARAMS
}