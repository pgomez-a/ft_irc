#include "Join.hpp"
#include "word_composition.hpp"

Join::Join(void) {
	_command_name = "JOIN";
	_id = JOIN;
};


Channel	*Join::get_channel(void)
{
	return &(_channel_iterator->second);
}

int	Join::_effect(server_t &server, client_t &client)
{
	int			r = 0;
	std::list<std::string>	token_list;

	if (_argc)
	{
		token_list = split_token(',', _argt[0]);
		for (std::list<std::string>::iterator channel = token_list.begin(); channel != token_list.end(); ++channel)
		{
			_channel_iterator = server.find_channel(*channel);
			if (_channel_iterator == server.channel_map_end())
				r = server.add_new_channel(*channel, "", "", _channel_iterator);
			if (r == BAD_CHANNEL_NAME)
				return ERR_NOSUCHCHANNEL;
			r = _channel_iterator->second.add_member(&client);
			if (r == BANNED)
				return ERR_BANNEDFROMCHAN;
			if (r == CHANNEL_IS_FULL)
				return ERR_CHANNELISFULL;
			_rest = _channel_iterator->second.get_topic();
			send_to_client( ":" + client.get_originname() + " JOIN :" + _channel_iterator->second.get_name() + "\r\n", client);
			reply_to_client( _rest.size() ? RPL_TOPIC: RPL_NOTOPIC, client, server, this);
			reply_to_client( RPL_NAMREPLY, client, server, this);
			reply_to_client( RPL_ENDOFNAMES, client, server, this);
		}
		return 0;
	}
	return ERR_NEEDMOREPARAMS;
}
