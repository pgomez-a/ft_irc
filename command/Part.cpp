#include "Part.hpp"

Part::Part(void) {
	_command_name = "PART";
	_id = PART;
};

int	Part::_effect(server_t &server, client_t &client)
{
	std::list<std::string>	token_list;
	bool			r;

	if (_argc)
	{
		token_list = split_token(',', _argt[0]);
		for (std::list<std::string>::iterator channel = token_list.begin(); channel != token_list.end(); ++channel)
		{
			_channel_iterator = server.find_channel(*channel);
			if (_channel_iterator == server.channel_map_end())
				return ERR_NOSUCHCHANNEL;
			r = _channel_iterator->second.delete_member(client.get_nick());
			if (!r)
				return ERR_NOTONCHANNEL;
			_channel_iterator->second.broadcast_message(client, "PART", *channel);
		}
		return 0;
	}
	return ERR_NEEDMOREPARAMS;
}
