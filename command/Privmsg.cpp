#include "Privmsg.hpp"
#include "word_composition.hpp"

Privmsg::Privmsg(void) {
	_command_name = "PRIVMSG";
	_id = PRIVMSG;
};

int	Privmsg::_effect(server_t &server, client_t &client)
{
	client_t*				receiver;
	std::list<std::string>			token_list;
	std::list<std::string>::iterator	nick;

	if (!_argc)
		return ERR_NORECIPIENT;
	if (_rest.size() == 0)
		return ERR_NOTEXTTOSEND;
	token_list = split_token(',', _argt[0]);
	nick = token_list.begin();
	while (nick != token_list.end())
	{
		receiver = server.find_nick(*nick);
		if (!receiver)
			return ERR_NOSUCHNICK;
		send_to_client( ":" + client.get_originname() + " PRIVMSG " + *nick + " :" + _rest + "\r\n", *receiver);
		++nick;
	}
	return 0;
}
