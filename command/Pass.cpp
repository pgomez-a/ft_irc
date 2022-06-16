#include "Pass.hpp"

Pass::Pass(void) {
	_command_name = "PASS";
	_id = PASS;
};

int	Pass::_effect(server_t &server, client_t &client)
{
	if (!_argc)
		return ERR_NEEDMOREPARAMS;
	if (client.registred_connection)
		return ERR_ALREADYREGISTERED;
	if (_argt[0] == server.passwd)
	{
		client.registred_connection = true;
		return RPL_WELCOME;
	}
	return 0;
}
