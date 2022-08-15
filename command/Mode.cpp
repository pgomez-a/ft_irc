#include "Mode.hpp"

Mode::Mode(void) :
_flag_change_table()
{
	_command_name = "MODE";
	_id = MODE;
};

Channel	*Mode::get_channel(void)
{
	return &(_channel_iterator->second);
}

void	Mode::_reset_flag_table(void)
{
	for (int i = 0; i < NO_OF_POSSIBLE_FLAGS; ++i)
	{
		_flag_change_table[i].change_type = DO_NOTHING;
	}
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

//no tokenization needed in this case
mode_parse_product	Mode::_user_mode_parser(std::string input)
{
	mode_parse_product p;
	int	change_type;

	for (int i = 0; input[i] != '\0'; ++i)
	{
		if (input[i] == '+' || input[i] == '-')
		{

			std::cout << "CHANGE TYPE FOUND '" << input[i] << "'\n";
			change_type = input[i];
			while (letter(input[i])) //filter valid flag types here
			{
				_flag_change_table[i].change_type = change_type;
				i++;
			}
		}
	}
	return p;
}

int	Mode::_change_user_modes(client_t &client)
{
	int	r = 0;

	for (int i = FIRST_POS_FLAG; i < NO_OF_POSSIBLE_FLAGS; ++i)
	{
		if (_flag_change_table[i].change_type == ADD_FLAG)
		{
			r = client.add_mode_flag(std::string(1, i)); //add more logic to flag addition, it shall return an error when necessary
		}
		else if (_flag_change_table[i].change_type == DROP_FLAG)
		{
			r = client.rm_mode_flag(std::string(1, i));
		}
		if (r)
			return r;
	}
	return RPL_UMODEIS;
}

int Mode::_user_mode(client_t &client)
{
	bool				correct_nick;

	correct_nick = (_argt[0] == client.get_nick());

	if (_argc == 1 && correct_nick)
		return RPL_UMODEIS; 
	if (!correct_nick)
		return ERR_USERSDONTMATCH;
	//extract mode addition or substraction
	//validate string *( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) )
	_user_mode_parser(_argt[0]);
	return _change_user_modes(client);
}

int	Mode::_effect(server_t &server, client_t &client)
{
	//hardcoded
	if (_argc)
	{
		_reset_flag_table();
		if (_argt[0][0] == '#')
		{
			return _channel_mode(server, client);
		}
		return _user_mode(client);
	}
	return ERR_NEEDMOREPARAMS;
}
