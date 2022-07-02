#include "Join.hpp"

Join::Join(void) {
	_command_name = "JOIN";
	_id = JOIN;
};


int	Join::_effect(server_t &server, client_t &client)
{
	if (_argc)
	{
		server.add_new_channel("test_channel");
	}
	return ERR_NEEDMOREPARAMS;
}
