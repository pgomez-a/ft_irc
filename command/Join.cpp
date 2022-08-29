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

int	Join::_join_loop(server_t &server, client_t &client, std::list<std::string> token_list)
{
	int			r = 0;
	bool		o = false;

	for (std::list<std::string>::iterator channel = token_list.begin(); channel != token_list.end(); ++channel)
		{
			_channel_iterator = server.find_channel(*channel);
			if (_channel_iterator == server.channel_map_end())
			{
				r = server.add_new_channel(*channel, "nt", "", _channel_iterator);
				o = true;
			}
			if (r == BAD_CHANNEL_NAME)
				return ERR_NOSUCHCHANNEL;
			r = _channel_iterator->second.add_member(&client);
			if (o)
				add_flag(client.get_joined_channel(*channel)->mode, 'O');
			if (r == MEMBER_DUPLICATE)
				continue ;
			if (r == BANNED)
				return ERR_BANNEDFROMCHAN;
			if (r == CHANNEL_IS_FULL)
				return ERR_CHANNELISFULL;
			_rest = _channel_iterator->second.get_topic();
			send_to_client( ":" + client.get_originname() + " JOIN :" + _channel_iterator->second.get_name() + "\r\n", client);
			_argt[0] = *channel;
			reply_to_client( _rest.size() ? RPL_TOPIC: RPL_NOTOPIC, client, server, this);
			reply_to_client( RPL_NAMREPLY, client, server, this);
			reply_to_client( RPL_ENDOFNAMES, client, server, this);
			_channel_iterator->second.broadcast_message(client, "JOIN", *channel);
		}
		return 0;
}

int	Join::_effect(server_t &server, client_t &client)
{
	std::list<std::string>	token_list;

	if (_argc)
	{
		if (_argt[0] == "0")
		{
			client.clear_channel_list();
			return 0;
		}
		token_list = split_token(',', _argt[0]);
		return _join_loop(server, client, token_list);
	}	
	return ERR_NEEDMOREPARAMS;
}
