#include "channel.hpp"

Channel::Channel(std::string name, std::string mode, std::string topic):
_member_list(), _member_count(0), _banned_list(), _name(name), _mode(mode), _topic(topic) {}


Channel::~Channel(void)
{
	//std::cout << "IM CHAN DESTRUCTOR\n";
	//std::cout << "it's name was : " << _name << std::endl;
}


//void broadcast_message(const std::string &message) const {}
int				Channel::add_member(client_t *member)
{
	if (_is_banned(member->get_nick()))
		return BANNED;
	if (!_find_member(member->get_nick()))
	{
		if (_member_count + 1 > MAX_CHANNEL_SIZE)
			return CHANNEL_IS_FULL;
		_member_list.push_back(member);
		++_member_count;
		return MEMBER_ADDED;
	}
	return MEMBER_DUPLICATE;
}

int		Channel::ban_member(const std::string &nick)
{
	if (_delete_member(nick) == true)
	{
		_banned_list.push_back(nick);
		return MEMBER_HAS_BEEN_BANNED;
	}
	return MEMBER_NOT_IN_CHANNEL;
}

void			Channel::set_topic(const std::string &topic){_topic = topic;}
std::string		Channel::get_topic(void) const {return _topic;}
std::string		Channel::get_name(void) const {return _name;}


//std::string			Channel::_channel_message_format(std::string message) const{}

client_t			*Channel::_find_member(std::string nick) 
{
	std::list<client_t *>::iterator end = _member_list.end();

	for (std::list<client_t *>::iterator i = _member_list.begin(); i != end; ++i)
	{
		if ((*i)->get_nick() == nick)
			return *i;
	}
	return NULL;
}

bool	Channel::_is_banned(std::string nick) const
{
	std::list<std::string>::const_iterator end = _banned_list.end();

	for (std::list<std::string>::const_iterator i = _banned_list.begin(); i != end; ++i)
	{
		if (*i == nick)
			return true;
	}
	return false;

}

bool			Channel::_delete_member(std::string nick)
{
	std::list<client_t *>::iterator end = _member_list.end();

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


