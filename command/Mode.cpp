#include "Mode.hpp"

Mode::Mode(void) {
	_command_name = "MODE";
	_id = MODE;
};

int	Mode::_effect(server_t &server, client_t &client)
{
	(void)server,(void)client;
	return 0;
}
