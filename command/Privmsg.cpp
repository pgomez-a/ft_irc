#include "Privmsg.hpp"
#include "word_composition.hpp"

Privmsg::Privmsg(void) {
	_command_name = "PRIVMSG";
	_id = PRIVMSG;
};

Channel	*Privmsg::get_channel(void)
{
	return &(_channel_iterator->second);
}

int	Privmsg::_effect(server_t &server, client_t &client)
{
	client_t*					receiver;
	std::list<std::string>				token_list;

	if (!_argc)
		return ERR_NORECIPIENT;
	if (_rest.size() == 0)
		return ERR_NOTEXTTOSEND;
	token_list = split_token(',', _argt[0]);
	for (std::list<std::string>::iterator nick = token_list.begin(); nick != token_list.end(); ++nick)
	{
		if ((*nick)[0] == '#')
		{
			_channel_iterator = server.find_channel(*nick);
			if (_channel_iterator == server.channel_map_end())
			{
				_rest = *nick;
				return ERR_NOSUCHCHANNEL;	
			}
			if (!client.is_in_channel(_channel_iterator->second))
				return ERR_CANNOTSENDTOCHAN;
			std::cout << "PRIVMSG : here\n";
			_channel_iterator->second.broadcast_message(client, "PRIVMSG", _rest);	
		}
		else
		{
			receiver = server.find_nick(*nick);
			if (!receiver)
				return ERR_NOSUCHNICK;
			send_to_client( ":" + client.get_originname() + " PRIVMSG " + *nick + " :" + _rest + "\r\n", *receiver);
		}
	}
	return 0;
}
