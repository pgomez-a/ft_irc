#include "Notice.hpp"

Notice::Notice(void) {
	_command_name = "NOTICE";
	_id = NOTICE;
};

int	Notice::_effect(server_t &server, client_t &client)
{
	client_t*	receiver;

	if (_argc && _rest.size())
	{
		receiver = server.find(_argt[0]);
		if (receiver)
			send_to_client(client.get_originname() + " NOTICE " + _argt[0] + " :" + _rest + "\r\n", *receiver);
	}
	return 0;
}
