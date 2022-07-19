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
		//add users
		return (_rest.size()) ? RPL_TOPIC: RPL_NOTOPIC;
	}
	(void)client;
	return ERR_NEEDMOREPARAMS;
}
