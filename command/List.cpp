#include "List.hpp"

List::List(void) {
	_command_name = "LIST";
	_id = LIST;
};

Channel	*List::get_channel(void)
{
	return &(_channel_iterator->second);
}


void	List::_complete_channel_list_string(server_t &server, client_t &client)
{
	server_t::channel_map::iterator i = server.channel_map_begin();
	server_t::channel_map::iterator j = server.channel_map_end();

	while (i != j)
	{
		_channel_iterator = i;
		reply_to_client(RPL_LIST, client, server, this);
		++i;
	}
	if (i != server.channel_map_begin())
		reply_to_client(RPL_LISTEND, client, server, this);
}

void	List::_channel_list_string(server_t &server, client_t &client)
{
	std::list<std::string>				channel_list;
	bool								p;

	p = false;	
	channel_list = split_token(',', _argt[0]);
	for (std::list<std::string>::iterator channel_name = server.channel_map_begin(); channel_name != server.channel_map_end(); ++channel_name)
	{
		channel = server.find_channel(*channel_name);
		if (channel->first)
		{
			p = true;
			_channel_iterator = channel->second();
			reply_to_client(RPL_LIST, client, server, this);
		}
		++i;
	}
	if (p)
		reply_to_client(RPL_LISTEND, client, server, this);
}

int	List::_effect(server_t &server, client_t &client)
{
	if (_argc)
		_channel_list(_argt[0], server, client)	
	else
		_channel_list_string(server, client);
	return 0;
}
