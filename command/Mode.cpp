#include "Mode.hpp"
#include "client.hpp"



/** b chan MODE flag **/

static void add_ban_flag(client_t &c, joined_channel *chan, std::list<std::string> argl)
{
	for (std::list<std::string>::iterator i = argl.begin(); i != argl.end(); ++i)
	{
		if (!chan->chan->is_banned(*i))
		{
			if (chan->chan->ban(*i, c.get_nick()))
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
		return ERR_NOTONCHANNEL;
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
}
/*****************/

/** t chan MODE flag **/
int	mode_restrict_topic(server_t &s, client_t &c, std::string channel_name, std::list<std::string> argl, int change, Command *n)
{
	joined_channel		*j = c.get_joined_channel(channel_name);
	bool				restricted_topic = false;

	restricted_topic = j->chan->mode_flag_is_set("t");	
	if (!j)
		return ERR_NOTONCHANNEL;
	if (is_operator(j->mode) || c.mode_flag_is_set("O"))
	{	
		if (!restricted_topic && change == ADD_FLAG)
		{
			j->chan->add_mode_flag("t");
			j->chan->broadcast_message(c, "MODE", "+t", 1);
		}
		if (restricted_topic && change == DROP_FLAG)
		{
			j->chan->rm_mode_flag("t");
			j->chan->broadcast_message(c, "MODE", "-t", 1);
		}
		return 0;
	}
	(void)s, (void)argl, (void)n;
	return ERR_CHANOPRIVSNEEDED;
}

/*****************/

/** n chan MODE flag **/


int	mode_restrict_message (server_t &s, client_t &c, std::string channel_name, std::list<std::string> argl, int change, Command *n)
{
	joined_channel		*j = c.get_joined_channel(channel_name);
	bool				restricted_msg = false;

	restricted_msg = j->chan->mode_flag_is_set("n");	
	if (!j)
		return ERR_NOTONCHANNEL;
	if (is_operator(j->mode) || c.mode_flag_is_set("O"))
	{	
		if (!restricted_msg && change == ADD_FLAG)
		{
			j->chan->add_mode_flag("n");
			j->chan->broadcast_message(c, "MODE", "+n", 1);
		}
		if (restricted_msg && change == DROP_FLAG)
		{
			j->chan->rm_mode_flag("n");
			j->chan->broadcast_message(c, "MODE", "-n", 1);
		}
		return 0;
	}
	(void)s, (void)argl, (void)n;
	return ERR_CHANOPRIVSNEEDED;
}

/*****************/
/*****************/

/** o chan MODE flag **/

int	chan_oper_flag(server_t &s, client_t &c, std::string channel_name, std::list<std::string> argl, int change, Command *n)
{
	joined_channel		*j = c.get_joined_channel(channel_name);
	joined_channel		*k;
	client_t			*t;
	bool				 is_chan_oper = false;

	if (argl.empty())
		return ERR_NEEDMOREPARAMS;
	if (!j)
		return ERR_NOTONCHANNEL;
	if (!(is_operator(j->mode) || c.mode_flag_is_set("O")))
		return ERR_CHANOPRIVSNEEDED;
	for (std::list<std::string>::iterator i = argl.begin(); i != argl.end() ; ++i)
	{
		t = j->chan->get_member(*i);
		if (!t)
		{
			reinterpret_cast<Mode*>(n)->mode_aux_str = *i;
		   	reply_to_client(ERR_USERNOTINCHANNEL, c, s, n);
		}
		else
		{	
			k = t->get_joined_channel(channel_name);
			is_chan_oper = flag_is_set(k->mode, 'o');
			if (!is_chan_oper && change == ADD_FLAG)
			{
				add_flag(k->mode, 'o');
				j->chan->broadcast_message(c, "MODE", *i, 1, " +o");
			}
			if (is_chan_oper && change == DROP_FLAG)
			{
				rm_flag(k->mode, 'o');
				if (!is_operator(t->get_mode()))
					j->chan->broadcast_message(c, "MODE", *i, 1, " -o");
			}
		}
	}
	return 0;
	(void)s, (void)argl, (void)n;
}

/*****************/
/** O user MODE flag **/

int	local_oper_flag(server_t &s, client_t &c, std::string user_name, std::list<std::string> argl, int change, Command *n)
{
	std::list<joined_channel> channel_list;

	if (change == DROP_FLAG)
	{
		if (flag_is_set((c.get_mode()), 'O'))
		{	
			c.rm_mode_flag("O");
			channel_list = c.get_joined_channel_list();
			for (std::list<joined_channel>::iterator i = channel_list.begin(); i != channel_list.end(); ++i)
			{
				if (!is_operator(i->mode))
				{
					i->chan->broadcast_message(c, "MODE", c.get_nick(), 1, " -o");
				}
			}
		}
	}
	return 0;
	(void)s, (void)user_name, (void)argl, (void)n;
}

/*****************/


/*****************/
/** i user MODE flag **/

int	invisible_flag(server_t &s, client_t &c, std::string user_name, std::list<std::string> argl, int change, Command *n)
{
	if (change == DROP_FLAG && flag_is_set((c.get_mode()), 'i'))
		c.rm_mode_flag("i");
	else
		c.add_mode_flag("i");
	return 0;
	(void)s, (void)user_name, (void)argl, (void)n;
}

/*****************/

/*****************/
/** w user MODE flag **/

int	wall_flag(server_t &s, client_t &c, std::string user_name, std::list<std::string> argl, int change, Command *n)
{
	if (change == DROP_FLAG && flag_is_set((c.get_mode()), 'w'))
		c.rm_mode_flag("w");
	else
		c.add_mode_flag("w");
	return 0;
	(void)s, (void)user_name, (void)argl, (void)n;
}

/*****************/
Mode::Mode(void) :
_chan_flag_table(), _user_flag_table()
{
	_command_name = "MODE";
	_id = MODE;
	//channel mode flags//
	_chan_flag_table[(int)'b'].apply = mode_ban_flag;
	_chan_flag_table[(int)'t'].apply = mode_restrict_topic;
	_chan_flag_table[(int)'n'].apply = mode_restrict_message;
	_chan_flag_table[(int)'o'].apply = chan_oper_flag;
	/////////////////////
	//user modes//
	_user_flag_table[(int)'O'].apply = local_oper_flag;
	_user_flag_table[(int)'i'].apply = invisible_flag;
	_user_flag_table[(int)'w'].apply = wall_flag;
	/////////////
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
		_chan_flag_table[(int)input[i][j - 1]].arg_list.push_back(input[i + k]);
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
			while (input[i][j] && channel_mode_flag(input[i][j]))
			{
				_chan_flag_table[(int)input[i][j]].change_type = change_type;
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

void	Mode::_reset_flag_table(std::string change, mode_change *flag_table)
{
	for (int i = 0; change[i]; ++i)
	{
		flag_table[(int)change[i]].change_type = DO_NOTHING;
		flag_table[(int)change[i]].change_count = 0;
		flag_table[(int)change[i]].arg_list.clear();
	}
}

int	Mode::_apply_channel_modes(server_t &server, client_t &client, parsed_instructions p)
{
	std::list<std::string>	*arg_list;
	mode_change				*m;
	int						r;

	for (int i = 0; p[i]; ++i)
	{
		m = &_chan_flag_table[(int)p[i]];
		if (m->change_count < 3)
		{
			arg_list = &m->arg_list;
			r = m->apply(server, client, _argt[0], *arg_list, m->change_type, this);
			reply_to_client(r, client, server, this);
			m->change_count++;
		}
	}
	_reset_flag_table(p, _chan_flag_table);
	return 0;
	(void)server,(void)client,(void)p;
}

int Mode::_channel_mode(server_t &server, client_t &client)
{
	_channel_iterator = server.find_channel(_argt[0]);
	std::list<std::string>	token_list;
	int						err;
	parsed_instructions		p;


	if (_channel_iterator == server.channel_map_end())
		return 0;
	if (_argc == 1)
		return RPL_CHANNELMODEIS;
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
				_user_flag_table[(int)input[i]].change_type = change_type;
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

int	Mode::_apply_user_modes(server_t &server, client_t &client, parsed_instructions &p)
{
	mode_change *m;
	int	r = 0;

	for (int i = 0; p[i]; ++i)
	{
		m = &_user_flag_table[(int)p[i]];
		r = m->apply(server, client, _argt[0], std::list<std::string>(), m->change_type, this);
		reply_to_client(r, client, server, this);
	}
	_reset_flag_table(p, _user_flag_table);
	return RPL_UMODEIS;
}

int Mode::_user_mode(server_t &server, client_t &client)
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
	if (r == VALID_UMODE)
		return _apply_user_modes(server, client, p);
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
		return _user_mode(server, client);
	}
	return ERR_NEEDMOREPARAMS;
}
