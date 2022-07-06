#include "Join.hpp"

Join::Join(void) {
	_command_name = "JOIN";
	_id = JOIN;
};

/*
 * If a JOIN is successful, the user receives a JOIN message as
   confirmation and is then sent the channel's topic (using RPL_TOPIC) and
   the list of users who are on the channel (using RPL_NAMREPLY), which
   MUST include the user joining.
  *
*/
int	Join::_effect(server_t &server, client_t &client)
{
	server_t::channel_map::iterator	i;
	int		r = 0;

	if (_argc)
	{
		i = server.find_channel(_argt[0]);
		if (i == server.channel_map_end())
			r = server.add_new_channel( _argt[0], "", "", i);
		if (r == BAD_CHANNEL_NAME)
			return ERR_NOSUCHCHANNEL;
		//add users
		_rest = i->second.get_topic();
		return (_rest.size()) ? RPL_TOPIC: RPL_NOTOPIC;
	}
	(void)client;
	return ERR_NEEDMOREPARAMS;
}
