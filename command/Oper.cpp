#include "Oper.hpp"

Oper::Oper(void) {
	_command_name = "OPER";
	_id = OPER;
};


int	Oper::_effect(server_t &server, client_t &client)
{
	if (_argc >= 2)
	{
		if (_argt[1] != server.oper_passwd)
			return ERR_PASSWDMISMATCH;
		if	(valid_oper_credentials(_argt[0], server, client) && !client.mode_flag_is_set("O"))
		{
			client.add_mode_flag("O");
			reply_to_client(RPL_YOUREOPER, client, server, this);
			return RPL_UMODEIS;
		}
		return 0;
	}
	return ERR_NEEDMOREPARAMS;
}
