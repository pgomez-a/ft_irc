#include "channel.hpp"
#include "reply_system.hpp"
#include "commands.hpp"

/** Constructor & Destructor **/
Channel::Channel(std::string name, std::string mode = "nt", std::string topic):
_member_list(), _member_count(0), _banned_list(), _name(name), _mode(mode), _topic(topic)
{}

Channel::~Channel(void)
{}

/** Member Methods **/
void	Channel::broadcast_message(client_t &sender, std::string command, const std::string &message, std::string optional) const 
{
	for (std::list<client_t *>::const_iterator member = _member_list.begin(); member != _member_list.end(); ++member)
	{
		if ((*member)->sock_fd != sender.sock_fd)
			send_to_client( ":" + sender.get_originname() + " " + command + " " +  _name + optional + " :" + message + "\r\n", *(*member));
	}	
	return ;
}

void	Channel::broadcast_message(client_t &sender, std::string command, const std::string &message, bool send_to_self, std::string optional) const 
{
	send_to_self ^= send_to_self;

	for (std::list<client_t *>::const_iterator member = _member_list.begin(); member != _member_list.end(); ++member)
	{
		send_to_client( ":" + sender.get_originname() + " " + command + " " +  _name + optional + " :" + message + "\r\n", *(*member));
	}	
	return ;
}

void	Channel::broadcast_message(client_t &sender, std::string message, bool send_to_self)
{
	for (std::list<client_t *>::const_iterator member = _member_list.begin(); member != _member_list.end(); ++member)
	{
		if (send_to_self || (*member)->sock_fd != sender.sock_fd)
			send_to_client( ":" + message + "\r\n", *(*member));
	}	
}

int		Channel::add_member(client_t *member)
{
	if (is_banned(member))
		return BANNED;
	if (!_find_member(member->get_nick()))
	{
		if (_member_count + 1 > MAX_CHANNEL_SIZE)
			return CHANNEL_IS_FULL;
		_member_list.push_back(member);
		member->add_channel_to_list(this);
		++_member_count;
		return MEMBER_ADDED;
	}
	return MEMBER_DUPLICATE;
}

bool	Channel::delete_member(std::string nick)
{
	std::list<client_t *>::iterator end;

	end = _member_list.end();
	for (std::list<client_t *>::iterator i = _member_list.begin(); i != end; ++i)
	{
		if ((*i)->get_nick() == nick)
		{	
			_member_list.erase(i);
			--_member_count;
			return true;
		}
	}
	return false;
}


ban_t::ban_t(std::string b, std::string o):
ban_mask(b), origin(o)
{}

static bool	valid_ban_mask(std::string m)
{
	size_t	exclamation = m.find('!');
	size_t	alt = m.find('@');

	return (m.size() && exclamation != std::string::npos && alt != std::string::npos && 
			exclamation < alt && ((alt - exclamation) > 0 && (m.size() - alt) > 0)); 
}

int		Channel::ban(const std::string ban_mask, std::string ban_origin)
{
	if (valid_ban_mask(ban_mask))
	{
		_banned_list.push_back(ban_t(ban_mask, ban_origin));
		return MEMBER_HAS_BEEN_BANNED;
	}	
	return 0;
}

void	Channel::unban(const std::string ban_mask)
{
	for (std::list<ban_t>::iterator i = _banned_list.begin(); i != _banned_list.end(); ++i)
	{
		if (ban_mask == i->ban_mask)
			i = _banned_list.erase(i);
	}
}

bool	Channel::is_member_on_channel(std::string nick)
{
	return (_find_member(nick) != NULL);
}

bool		Channel::is_banned(client_t *client) const
{
	std::string		client_oname;
	std::string		cur_ban_mask;

	for (std::list<ban_t>::const_iterator i = _banned_list.begin(); i != _banned_list.end(); ++i)
	{
		client_oname = client->get_originname();
		cur_ban_mask = i->ban_mask;
		if (cur_ban_mask[0] == '*')
		{
			cur_ban_mask = cur_ban_mask.substr(cur_ban_mask.find('!'));
			client_oname = client_oname.substr(client_oname.find('!'));
		}
		if (cur_ban_mask == client_oname)
			return true;
	}
	return false;
}

bool	Channel::is_banned(std::string oname) const
{
	std::string		cur_ban_mask;
	std::string		client_oname;


	if (!valid_ban_mask(oname))
		return false;
	for (std::list<ban_t>::const_iterator i = _banned_list.begin(); i != _banned_list.end(); ++i)
	{
		client_oname = oname;
		cur_ban_mask = i->ban_mask;
		if (cur_ban_mask[0] == '*')
		{
			cur_ban_mask = cur_ban_mask.substr(1);
			client_oname = client_oname.substr(client_oname.find('!'));
		}
		if (cur_ban_mask == client_oname)
			return true;
	}
	return false;
}


int		Channel::add_mode_flag(std::string flag)
{
	if (!channel_mode_flag(flag.front()))
		return ERR_UNKNOWNMODE;
	if (_mode.find(flag) == std::string::npos)
		_mode += flag;
	return 0;
}

int		Channel::rm_mode_flag(std::string flag)
{
	size_t pos;

	pos = _mode.find(flag);
	if (pos != std::string::npos)
		_mode.erase(pos, flag.size());
	return 0;
}

bool	Channel::mode_flag_is_set(std::string flag)
{
	return _mode.find(flag) != std::string::npos;
}

/** Getters & Setters **/
void		Channel::set_topic(const std::string &topic)
{
	_topic = topic;
}

std::string	Channel::get_topic(void) const { return _topic;}

std::string	Channel::get_name(void) const { return _name;}

size_t		Channel::get_member_count(void) const { return _member_count;}

client_t	*Channel::get_member(std::string nick)
{
	for (std::list<client_t *>::iterator i = _member_list.begin(); i != _member_list.end(); ++i)
	{
		if ((*i)->get_nick() == nick)
			return (*i);
	}
	return NULL;
}

std::list<client_t *> Channel::get_member_list(void)
{
	return _member_list;
}

std::string	Channel::get_member_list(char separator)
{
	std::string list;

	for (std::list<client_t *>::iterator i = _member_list.begin(); i != _member_list.end(); ++i)
	{
		if ( (*i)->mode_flag_is_set("O") || is_operator((*i)->get_joined_channel(_name)->mode))
			list += "@";
		list += (*i)->get_nick() + separator;
	}
	list.pop_back();
	return list;
}

std::string	Channel::get_mode()
{
	if (_mode.size())
		return "+" + _mode;
	return "";
}

std::list<client_t *>	Channel::get_member_list_by_origin_name(std::string originname)
{
	std::list<client_t *>	member_list;
	bool					wildcard_nick;
	std::string				members_oname;

	if (originname[0] == '*')
	{
		wildcard_nick = true;
		originname = originname.substr(1);
	}
	for (std::list<client_t *>::iterator i = _member_list.begin(); i != _member_list.end(); ++i)
	{
		members_oname = (*i)->get_originname();
		if (wildcard_nick)
		{
			members_oname = members_oname.substr(members_oname.find('!'));
		}
		if (members_oname == originname )
			member_list.push_back(*i);
	}
	return member_list;
}

std::list<ban_t> Channel::get_banned_list(void)
{
	return _banned_list;
}

/** Private Methods **/
client_t	*Channel::_find_member(std::string nick) 
{
	std::list<client_t *>::iterator end;

	end = _member_list.end();
	
	for (std::list<client_t *>::iterator i = _member_list.begin(); i != end; ++i)
	{
		if ((*i)->get_nick() == nick)
			return *i;
	}
	return NULL;
}


