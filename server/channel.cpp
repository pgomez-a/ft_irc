#include "channel.hpp"
#include "reply_system.hpp"

/** Constructor & Destructor **/
Channel::Channel(std::string name, std::string mode = "nt", std::string topic):
_member_list(), _member_count(0), _banned_list(), _name(name), _mode(mode), _topic(topic)
{}

Channel::~Channel(void)
{}

/** Member Methods **/
void	Channel::broadcast_message(client_t &sender, std::string command, const std::string &message) const 
{
	for (std::list<client_t *>::const_iterator member = _member_list.begin(); member != _member_list.end(); ++member)
	{
		if ((*member)->sock_fd != sender.sock_fd)
			send_to_client( ":" + sender.get_originname() + " " + command + " " +  _name + " :" + message + "\r\n", *(*member));
	}	
	return ;
}

void	Channel::broadcast_message(client_t &sender, std::string command, const std::string &message, bool send_to_self) const 
{
	send_to_self ^= send_to_self;

	for (std::list<client_t *>::const_iterator member = _member_list.begin(); member != _member_list.end(); ++member)
	{
		send_to_client( ":" + sender.get_originname() + " " + command + " " +  _name + " :" + message + "\r\n", *(*member));
	}	
	return ;
}

int		Channel::add_member(client_t *member)
{
	if (_is_banned(member->get_nick()))
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

int		Channel::ban_member(const std::string &nick)
{
	if (delete_member(nick) == true)
	{
		_banned_list.push_back(nick);
		return MEMBER_HAS_BEEN_BANNED;
	}
	return MEMBER_NOT_IN_CHANNEL;
}

bool	Channel::is_member_on_channel(std::string nick)
{
	return (_find_member(nick) != NULL);
}

/** Getters & Setters **/
void		Channel::set_topic(const std::string &topic){_topic = topic;}

std::string	Channel::get_topic(void) const { return _topic;}

std::string	Channel::get_name(void) const { return _name;}

size_t		Channel::get_member_count(void) const { return _member_count;}

std::string	Channel::get_member_list(char separator)
{
	std::string list;

	for (std::list<client_t *>::iterator i = _member_list.begin(); i != _member_list.end(); ++i)
		list += (*i)->get_nick() + separator;
	list.pop_back();
	return list;
}

std::string	Channel::get_mode()
{
	if (_mode.size())
		return "+" + _mode;
	return "empty";
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

bool		Channel::_is_banned(std::string nick) const
{
	std::list<std::string>::const_iterator end;

	end = _banned_list.end();
	for (std::list<std::string>::const_iterator i = _banned_list.begin(); i != end; ++i)
	{
		if (*i == nick)
			return true;
	}
	return false;
}
