#include "channel.hpp"
#include "server.hpp"
#include "client.hpp"
#include "reply_system.hpp"
#include "parser.hpp"

/** Constructor & Destructor **/
client_t::client_t(void):
sock_fd(), addr(), port(), info(), _registration_flags(0), _mode(), _nick(), _user(), _realname()
{}

client_t::~client_t(void)
{}

/** Auxiliary struct Construct **/

joined_channel::joined_channel(Channel *channel, std::string mode)
{
	this->chan = channel;
	this->mode = mode;	
}

/** Operator Overloads **/
bool	client_t::operator==(client_t &rhs) const
{
	return this->port == rhs.port && this->addr == rhs.addr;
}

bool	client_t::operator==(const client_t &rhs) const
{
	return this->port == rhs.port && this->addr == rhs.addr;
}

/** Member Methods **/
void	client_t::register_flag(unsigned int flag)
{
	_registration_flags |= flag;
	return ;
}

bool	client_t::flag_is_set(unsigned int flag) const
{
	return _registration_flags & flag;
}

bool	client_t::registered(void) const
{
	return _registration_flags == IS_REGISTERED;
}

int	client_t::add_mode_flag(std::string flag)
{
	if (!user_mode_flag(flag.front()))
		return ERR_UMODEUNKNOWNFLAG;
	if (_mode.find(flag) == std::string::npos)
		_mode += flag;
	return 0;
}

int	client_t::rm_mode_flag(std::string flag)
{
	size_t pos;

	pos = _mode.find(flag);
	if (pos != std::string::npos)
		_mode.erase(pos, flag.size());
	return 0;
}

bool	client_t::mode_flag_is_set(std::string flag)
{
	return _mode.find(flag) != std::string::npos;
}

void	client_t::reset(int reset_mode)
{
	if (reset_mode == HARD_RESET)
		clear_channel_list();
	_registration_flags = 0;
	_mode.clear();
	_nick.clear();
	_user.clear();
	return ;
}

void	client_t::add_channel_to_list(Channel *c, std::string m)
{
	_channel_list.push_back(joined_channel(c, m));
	return ;
}

bool	client_t::pop_channel_from_list(Channel *c)
{
	for (std::list<joined_channel>::iterator i = _channel_list.begin(); i != _channel_list.end(); ++i)
	{
		if (i->chan->get_name() == c->get_name())
		{
			_channel_list.erase(i);
			return true;
		}
	}
	return false;
}

void	client_t::clear_channel_list(void)
{
	if (_channel_list.empty())
		return ;
	for (std::list<joined_channel>::iterator i = _channel_list.begin(); i != _channel_list.end(); ++i)
	{
		i->chan->broadcast_message(*this, "PART", i->chan->get_name());
		send_to_client( ":" + get_originname() + " PART " +  ":" + i->chan->get_name() + "\r\n", *this);
		i->chan->delete_member(_nick);
	}
	_channel_list.clear();
	return ;
}

bool	client_t::is_in_channel(Channel &c)
{
	for (std::list<joined_channel>::iterator i = _channel_list.begin(); i != _channel_list.end(); ++i)
	{
		if (i->chan->get_name() == c.get_name())
			return true;
	}
	return false;	
}


/** Getters & Setters **/
void		client_t::set_mode(std::string m) { _mode = m;}

std::string	client_t::get_mode(void) const { return _mode;}

void		client_t::set_nick(std::string s) { _nick = s;}

std::string	client_t::get_nick(void) const { return _nick;}

void		client_t::set_user(std::string s) { _user = s;}

std::string	client_t::get_user(void) const { return _user;}

void		client_t::set_realname(std::string s) { _realname = s;}

std::string	client_t::get_realname(void) const { return _realname;}

std::string	client_t::get_originname(void) const { return _nick + "!" + _user + "@" + addr;}

joined_channel	*client_t::get_joined_channel(std::string channel_name)
{
	joined_channel	*j = NULL;

	for (std::list<joined_channel>::iterator i = _channel_list.begin(); i != _channel_list.end(); ++i)
	{
		if (i->chan->get_name() == channel_name)
			j = &*i;
	}
	return j;
}

std::list<joined_channel>	client_t::get_joined_channel_list(void)
{
	return _channel_list;
}

/** Mode functions **/

bool		valid_oper_credentials(std::string username, server_t &server, client_t &client)
{
	return client.get_originname() == username && server.find_user(client.get_user()) && server.valid_oper_host(client);
}

std::string user_mode_bitmask(int m)
{
	char		w;
	char		i;
	std::string	mode;

	w = ('w' * (bool)(m & 4));
	i = ('i' * (bool)(m & 8));
	if (w)
		mode += "w";
	if (i)
		mode += "i";
	else
		mode = "+";
	return mode;
}

/** Interaction **/

bool client_is_in_server(const server_t &server, const client_t &client)
{
	for (size_t i = 0; i < MAX_CLIENTS; ++i)
	{
		if (server.clients_info[i] == client)
			return true;
	}
	return false;
}

bool client_is_registered(const server_t &server, const client_t &client)
{
	return client.registered() && client_is_in_server(server, client);
}
