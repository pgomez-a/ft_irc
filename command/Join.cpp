#include "Join.hpp"

Join::Join(void) {
	_command_name = "JOIN";
	_id = JOIN;
};


int	Join::_effect(server_t &server, client_t &client)
{

	if (_argc)
	{
		std::cout << "About to add a channel from join\n";
		int i = server.add_new_channel("#testchan", "", "This is a test channel");
		std::cout << "i=" << i << "channel count << " << server.channel_count() << std::endl;
		server.add_new_channel("#altchan", "", "This is another test channel");
		std::cout << "i=" << i << "channel count << " << server.channel_count() << std::endl;
		return 0;
	}
	(void)client;
	return ERR_NEEDMOREPARAMS;
}
