#include "Cap.hpp"

Cap::Cap(void) {
	_command_name = "CAP";
	_id = CAP;
};

int	Cap::_effect(server_t &server, client_t &client)
{	
	(void)server;
	if (_argc < 2)
		return ERR_NEEDMOREPARAMS;
	if (_argt[0] == "LS" || _argt[0] == "LIST")
	{
		send_to_client(reply_format("Server capabilities, :Nameless supports basic messaging and, uhm, that's it...\r\n"), client);
	}
	return 0;
}
