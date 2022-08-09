#include "Mode.hpp"

Mode::Mode(void) {
	_command_name = "MODE";
	_id = MODE;
};

Channel	*Mode::get_channel(void)
{
	return &(_channel_iterator->second);
}

int Mode::_channel_mode(server_t &server, client_t &client)
{
	_channel_iterator = server.find_channel(_argt[0]);

	if (_argc == 1)
		return RPL_CHANNELMODEIS;
	//validate string <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )
	(void)client;
	return 0;
}

int Mode::_user_mode(server_t &server, client_t &client)
{
	if (_argc == 1)
		return RPL_UMODEIS; //implement this reply
	//extract mode addition or substraction
	//validate string *( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) )
	return 0;
}

int	Mode::_effect(server_t &server, client_t &client)
{
	//hardcoded
	if (_argc)
	{
		if (_argt[0][0] == '#')
		{
			return _channel_mode(server, client);
		}
		return _user_mode(server, client);
	}
	return ERR_NEEDMOREPARAMS;
}
