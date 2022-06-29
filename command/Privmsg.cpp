#include "Privmsg.hpp"

Privmsg::Privmsg(void) {
	_command_name = "PRIVMSG";
	_id = PRIVMSG;
};

int	Privmsg::_effect(server_t &server, client_t &client)
{
	int		comma_pos;
	client_t*	receiver;
	std::string	nick;

	if (!_argc)
		return ERR_NORECIPIENT;
	if (_rest.size() == 0)
		return ERR_NOTEXTTOSEND;
	comma_pos = 0;
	while (_argt[0][comma_pos])
	{
		if (_argt[0][comma_pos] == ',')
		{
			receiver = server.find_nick(nick);
			if (!receiver)
				return ERR_NOSUCHNICK;
			send_to_client( ":" + client.get_originname() + " PRIVMSG " + nick + " :" + _rest + "\r\n", *receiver);
			nick.clear();
		}
		else
			nick += _argt[0][comma_pos];	
		comma_pos += 1;
	}
	receiver = server.find_nick(nick);
	if (!receiver)
		return ERR_NOSUCHNICK;
	send_to_client(":" + client.get_originname() + " PRIVMSG " + nick + " :" + _rest + "\r\n", *receiver);
	return 0;
}
