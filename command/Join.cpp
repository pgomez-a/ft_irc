#include "Join.hpp"

Join::Join(void) {
	_command_name = "JOIN";
	_id = JOIN;
};


Channel	*Join::get_channel(void)
{
	return &(_channel_iterator->second);
}

/*
 * If a JOIN is successful, the user receives a JOIN message as
   confirmation and is then sent the channel's topic (using RPL_TOPIC) and
   the list of users who are on the channel (using RPL_NAMREPLY), which
   MUST include the user joining.
  *
*/
int	Join::_effect(server_t &server, client_t &client)
{
	int		r = 0;

	if (_argc)
	{
		_channel_iterator = server.find_channel(_argt[0]);
		if (_channel_iterator == server.channel_map_end())
			r = server.add_new_channel( _argt[0], "", "", _channel_iterator);
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
		return RPL_ENDOFNAMES;
	}
	return ERR_NEEDMOREPARAMS;
}
