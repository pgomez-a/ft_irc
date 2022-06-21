#include "Nick.hpp"

Nick::Nick(void) {
	_command_name = "NICK";
	_id = NICK;
};

int	Nick::_effect(server_t &server, client_t &client)
{	
	bool		p;
	
	if (!_argc)
		return ERR_NONICKNAMEGIVEN;
	if  (valid_nickname(_argt[0]))
	{
		if (server.find(_argt[0]))
			return ERR_NICKNAMEINUSE;
		p = !client.registered();
		client.register_flag(CLI_NICK);
		client.set_nick(_argt[0]);
		if (!p)
			send_to_client(reply_format("You are now known as " + _argt[0]), client);
		return (p && client.registered()) ? welcome_new_registration(client, server, this) : 0;
	}
	return ERR_ERRONEUSNICKNAME;
}
