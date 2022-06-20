#include "User.hpp"

User::User(void) {
	_command_name = "USER";
	_id = USER;
};

int	User::_effect(server_t &server, client_t &client)
{
	//Parameters: <user> <mode> <unused> <realname>
	//<mode> is numeric, sets user modes on registration
	//It is a bitmask with 2 bits of 'signification':
	//if bit 2 is set, user mode 'w' is set, and if bit 3 is set, user mode 'i' is too.
	//(RFC 2812 section 3.1.5 for more info on modes)
	//
	//Example of USER command:
	//USER npinto-g 0 * :George W Bush
	//Makes the user register with username 'npinto-g',  without modes set and with real name
	//'George W Bush'
	(void)server, (void)client;
	client.register_flag(CLI_USER);
	client.set_user("dummy_user");
	if (client.registered())
		return welcome_new_registration(client, server, this);
	return 0;
}
