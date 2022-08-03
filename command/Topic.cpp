#include "Topic.hpp"

Topic::Topic(void) {
	_command_name = "TOPIC";
	_id = TOPIC;
};

Channel	*Topic::get_channel(void)
{
	return &(_channel_iterator->second);
}

int	Topic::_effect(server_t &server, client_t &client)
{

	Channel *channel;

	if (_argc)
	{
		_channel_iterator = server.find_channel(_argt[0]);	
		if (_channel_iterator == server.channel_map_end())
			return 0;
		channel = &_channel_iterator->second;
		if (!channel->is_member_on_channel(client.get_nick()))
				return ERR_NOTONCHANNEL;
		//guard checking channel  required permits
		if (rest_sent || _rest.size())
		{
			channel->set_topic(_rest);
		}
		return (_rest.empty())? RPL_NOTOPIC : RPL_TOPIC;
	}
	return ERR_NEEDMOREPARAMS;
}
