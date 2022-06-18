#include "Pass.hpp"

Pass::Pass(void) {
	_command_name = "PASS";
	_id = PASS;
};

int	Pass::_effect(server_t &server, client_t &client)
{
	if (!_argc)
		return ERR_NEEDMOREPARAMS;
	if (client.registered())
		return ERR_ALREADYREGISTRED;
	if (_argt[0] == server.passwd)
	{
		client.register_flag(SERV_PASSW);
		if (client.registered())
			return welcome_new_registration(client, server, this);
	}
	return 0;
}
