#include "Mode.hpp"


int	_mode_ban_flag(server_t &s, client_t &c, std::list<std::string> argl)
{
	std::cout << "WILL BAN U DAS RITE\n";
	(void)s,(void)c,(void)argl;
	return 0;
}


Mode::Mode(void) :
_flag_table()
{
	_command_name = "MODE";
	_id = MODE;
	//assign the apply method for each flag
	_flag_table[(int)'b'].apply = _mode_ban_flag;
};

Channel	*Mode::get_channel(void)
{
	return &(_channel_iterator->second);
}


std::string	Mode::get_last_mode_request(void)
{
	return _last_mode_request;
}

void	Mode::_extract_cmode_arg(int &i, int j, std::string *input)
{
	int k = 1;

	while (i + k <(int)_argc - 1)
	{
		if (input [i + k][0] == '+' || input[i + k][0] == '-')
			break ;
		_flag_table[(int)input[i][j - 1]].arg_list.push_back(input[i + k]);
		++k;
	}
	i += k;
}

int	Mode::_channel_mode_parser(std::string *input, parsed_instructions &p)
{
	int	change_type = DO_NOTHING;
	int i = 0;
	int	j = 0;

	while (i < (int)_argc - 1)
	{
		j = 0;
		if (input[i][j] == '+' || input[i][j] == '-')
		{
			change_type = (input[i][j] == '+');
			j++;
			while (input[i][j] && channel_mode_flag(input[i][j])) //implement chan_mode_flag
			{
		
				_flag_table[(int)input[i][j]].change_type = change_type;
				p += input[i][j];
				++j;
			}
			_last_mode_request = input[i];
			if (input[i][j] == '\0')
				_extract_cmode_arg(i, j, input);
			else
				return ERR_UNKNOWNMODE;
		}
		else 
		{
			return ERR_UNKNOWNMODE;	
		}
	}
	return 0;
}

void	Mode::_reset_flag_table(std::string change)
{
	for (int i = 0; change[i]; ++i)
	{
		_flag_table[(int)change[i]].change_type = DO_NOTHING;
		_flag_table[(int)change[i]].change_count = 0;
		_flag_table[(int)change[i]].arg_list.clear();
	}
}

int	Mode::_apply_channel_modes(server_t &server, client_t &client, parsed_instructions p)
{
	std::list<std::string> *arg_list;

	for (int i = 0; p[i]; ++i)
	{
		if (_flag_table[(int)p[i]].change_count < 3)
		{
			arg_list = &_flag_table[(int)p[i]].arg_list;
			_flag_table[(int)p[i]].apply(server, client,*arg_list);
			_flag_table[(int)p[i]].change_count++;
		}
	}
	_reset_flag_table(p);
	return 0;
	(void)server,(void)client,(void)p;
}

int Mode::_channel_mode(server_t &server, client_t &client)
{
	_channel_iterator = server.find_channel(_argt[0]);
	std::list<std::string>	token_list;
	int						err;
	parsed_instructions		p;


	if (_argc == 1)
		return RPL_CHANNELMODEIS;
	//validate string <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )
	err = _channel_mode_parser(_argt + 1, p);
	if (!err)
		return _apply_channel_modes(server, client, p);
	return err;
}

//no tokenization needed in this case
int	 Mode::_user_mode_parser(std::string input, parsed_instructions &p)
{
	int	change_type;
	int	i = 0;			

	while (input[i])
	{
		if (input[i] == '+' || input[i] == '-')
		{
			change_type = (input[i] == '+');
			i++;
			while (user_mode_flag(input[i]))
			{

				_flag_table[(int)input[i]].change_type = change_type;
				p += input[i];
				i++;
			}
		}
		if (!(input[i] == '\0' || input[i] == '+' || input[i] == '-'))
		{
			return INVALID_UMODE;
		}
	}
	return VALID_UMODE;
}

int	Mode::_apply_user_modes(client_t &client, parsed_instructions &p)
{
	int	r = 0;

	for (int i = 0; p[i]; ++i)
	{
		if (_flag_table[(int)p[i]].change_type == ADD_FLAG)
		{
			client.add_mode_flag(std::string(1, p[i]));
		}
		else
		{
			client.rm_mode_flag(std::string(1, p[i]));
		}
		if (r)
			return r;
	}
	return RPL_UMODEIS;
}

int Mode::_user_mode(client_t &client)
{
	parsed_instructions	p;
	bool				correct_nick;
	int					r;

	correct_nick = (_argt[0] == client.get_nick());

	if (_argc == 1 && correct_nick)
		return RPL_UMODEIS; 
	if (!correct_nick)
		return ERR_USERSDONTMATCH;
	r = _user_mode_parser(_argt[1], p);
	std::cout << "out with " << r << std::endl;
	if (r == VALID_UMODE)
		return _apply_user_modes(client, p);
	return ERR_UMODEUNKNOWNFLAG;
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
		return _user_mode(client);
	}
	return ERR_NEEDMOREPARAMS;
}
