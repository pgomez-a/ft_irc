#include "Mode.hpp"
#include "client.hpp"



/** b MODE flag **/

static void add_ban_flag(client_t &c, joined_channel *chan, std::list<std::string> argl)
{
	for (std::list<std::string>::iterator i = argl.begin(); i != argl.end(); ++i)
	{
		if (!chan->chan->is_banned(*i))
		{
			chan->chan->ban(*i, c.get_nick());
			chan->chan->broadcast_message(c, "MODE", "+b :" + *i, 1);
		}
	}
}

static void drop_ban_flag(client_t &c, joined_channel *chan, std::list<std::string> argl)
{
	for (std::list<std::string>::iterator i = argl.begin(); i != argl.end(); ++i)
	{
		if (chan->chan->is_banned(*i))
		{
			chan->chan->unban(*i);
			chan->chan->broadcast_message(c, "MODE", "-b :" + *i, 1);
		}
	}
}

int	mode_ban_flag(server_t &s, client_t &c, std::string channel_name, std::list<std::string> argl, int change, Command *n)
{
	joined_channel			*chan = c.get_joined_channel(channel_name);
	std::list<ban_t>		ban_list = chan->chan->get_banned_list();

	if (!chan)
		return ERR_NOTONCHANNEL;;
	if (argl.empty())
	{
		for (std::list<ban_t>::iterator i = ban_list.begin(); i != ban_list.end(); ++i)
		{
			n->set_aux_msg(i->ban_mask + " :" + i->origin);
		   reply_to_client(RPL_BANLIST, c, s, n);	   
		}
		return RPL_ENDOFBANLIST;
	}	
	if (is_operator(chan->mode) || c.mode_flag_is_set("O"))
	{
		if (change == ADD_FLAG)
			add_ban_flag(c, chan, argl);
		else 
			drop_ban_flag(c, chan, argl);
		return 0;
	}
	return ERR_CHANOPRIVSNEEDED;
	(void)s;
}
/*****************/

/** t MODE flag **/
int	mode_restrict_topic(server_t &s, client_t &c, std::string channel_name, std::list<std::string> argl, int change, Command *n)
{
	Channel	*chan = &s.find_channel(channel_name)->second;

	(void)s, (void)c, (void)chan, (void)channel_name, (void)argl, (void)change, (void)n;
	return ERR_CHANOPRIVSNEEDED;
}

Mode::Mode(void) :
_flag_table()
{
	_command_name = "MODE";
	_id = MODE;
	_flag_table[(int)'b'].apply = mode_ban_flag;
	_flag_table[(int)'t'].apply = mode_restrict_topic;
}

Channel	*Mode::get_channel(void)
{
	return &(_channel_iterator->second);
}


std::string	Mode::get_last_mode_request(void)
{
	return _last_mode_request;
}

std::string Mode::get_aux_msg(void) const
{
	return _ban_mask_msg;
}

void	Mode::set_aux_msg(std::string m)
{
	_ban_mask_msg = m;
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
	std::list<std::string>	*arg_list;
	mode_change				*m;
	int						r;

	for (int i = 0; p[i]; ++i)
	{
		m = &_flag_table[(int)p[i]];
		if (m->change_count < 3)
		{
			arg_list = &m->arg_list;
			r = m->apply(server, client, _argt[0], *arg_list, m->change_type, this);
			reply_to_client(r, client, server, this);
			m->change_count++;
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
