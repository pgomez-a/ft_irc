#include "List.hpp"

List::List(void) {
	_command_name = "LIST";
	_id = LIST;
};

std::string	_channel_list_string(server_t &server)
{
	channel_map::iterator i = server.channel_map_begin();
	channel_map::iterator j = server.channel_map_end();
	std::string s;

	while (i != j)
	{
		s += reply_format(i->second.get_nick());
		++i;
	}
	return (s.size()) ? s : "(server has no channels)\r\n";
}

int	List::_effect(server_t &server, client_t &client)
{
	if (!_argc)
		send_to_client(_channel_list_string(server), client);
	return 0;
}
