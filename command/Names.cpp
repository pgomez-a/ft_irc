#include "Names.hpp"
#include "word_composition.hpp"

Names::Names(void) {
	_command_name = "NAMES";
	_id = NAMES;
};


Channel	*Names::get_channel(void)
{
	return &(_channel_iterator->second);
}

int Names::_list_all_channels(server_t &s, client_t &c)
{
	for (_channel_iterator = s.channel_map_begin(); _channel_iterator != s.channel_map_end(); ++_channel_iterator)
	{
		reply_to_client( RPL_NAMREPLY, c, s, this);	
	}
	return RPL_ENDOFNAMES;
}	

int	Names::_list_channel_loop(server_t &server, client_t &client, std::list<std::string> token_list)
{
	for (std::list<std::string>::iterator channel = token_list.begin(); channel != token_list.end(); ++channel)
		{
			_channel_iterator = server.find_channel(*channel);
			if (_channel_iterator !=  server.channel_map_end())
				reply_to_client( RPL_NAMREPLY, client, server, this);
		}
		return RPL_ENDOFNAMES;
}

int	Names::_effect(server_t &server, client_t &client)
{
	std::list<std::string>	token_list;

	if (_argc)
	{
		token_list = split_token(',', _argt[0]);
		return _list_channel_loop(server, client, token_list);
	}
	return _list_all_channels(server, client);
}
