#include "List.hpp"

List::List(void) {
	_command_name = "LIST";
	_id = LIST;
};

std::string	List::_channel_list_string(server_t &server)
{
	server_t::channel_map::iterator i = server.channel_map_begin();
	server_t::channel_map::iterator j = server.channel_map_end();
	std::string s;

	std::cout << "mapsize : " << server.channel_count() << std::endl;
	while (i != j)
	{
		std::cout << "l: " << server.channel_count() << std::endl;
		s += reply_format(i->second.get_name());
		++i;
	}
	std::cout << "s.size()? " << s.size() << std::endl;
	return (s.size()) ? s : "(server has no channels)\r\n";
}

int	List::_effect(server_t &server, client_t &client)
{
	if (!_argc)
		send_to_client(_channel_list_string(server), client);
	return 0;
}
