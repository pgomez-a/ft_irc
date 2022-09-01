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
		if (channel->mode_flag_is_set("t") && !(is_operator(client.get_joined_channel(channel->get_name())->mode) || is_operator(client.get_mode())))
			return ERR_CHANOPRIVSNEEDED;
		if (rest_sent || _rest.size())
		{
			channel->set_topic(_rest);
			channel->broadcast_message(client, "TOPIC", channel->get_topic(), true);
			return 0;
		}
		return (channel->get_topic().empty())? RPL_NOTOPIC : RPL_TOPIC;
	}
	return ERR_NEEDMOREPARAMS;
}
