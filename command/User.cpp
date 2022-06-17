#include "User.hpp"

User::User(void) {
	_command_name = "USER";
	_id = USER;
};

int	User::_effect(server_t &server, client_t &client)
{
	//do someth;
	//
	(void)server, (void)client;
	return 0;
}
