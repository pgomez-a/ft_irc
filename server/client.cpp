#include "server.hpp"

bool client_is_in_server(const server_t &server, const client_t &client)
{
	//needs to be refactored, clients_info should be a map instead of an array, and client_t should have
	//comparison overload among other things
	for (size_t i = 0; i < MAX_CLIENTS; ++i)
	{
		if (server.clients_info[i] == client)
			return true;
	}
	return false;
}

bool client_is_registered(const server_t &server, const client_t &client)
{
	return (client.registered() && client_is_in_server(server, client));
}


/* Default constructor */
client_t::client_t(void):
sock_fd(), addr(), port(), info(), 
_registration_flags(0), _mode(), _nick(), _user() {}

void	client_t::register_flag(unsigned int flag)
{
	_registration_flags |= flag;
}

bool client_t::flag_is_set(unsigned int flag) const
{
	return (_registration_flags & flag);
}

bool client_t::registered(void) const
{
	return (_registration_flags == IS_REGISTERED);
}

void	client_t::set_mode(std::string m) 
{
	_mode = m;	
}

std::string	client_t::get_mode(void) const 
{
	return _mode;
}

void	client_t::add_mode_flag(std::string flag)
{
	if (_mode == "empty")
		_mode.clear();
	if (_mode.find(flag) == std::string::npos)
		_mode += flag;
}

void	client_t::rm_mode_flag(std::string flag)
{
	size_t pos = _mode.find(flag);

	if (pos != std::string::npos)
	{
		_mode.erase(pos, flag.size());
	}
}

bool	client_t::mode_flag_is_set(std::string flag)
{
	return (_mode.find(flag) != std::string::npos);
}

void	client_t::set_nick(std::string s) { _nick = s;}

std::string	client_t::get_nick(void) const { return _nick;}

void	client_t::set_user(std::string s) { _user = s;}

std::string	client_t::get_user(void) const { return _user;}

void	client_t::set_realname(std::string s) { _realname = s;}

std::string	client_t::get_realname(void) const { return _realname;}

std::string	client_t::get_originname(void) const
{
	return (_nick + "!" + _user + "@" + addr);
}

void	client_t::reset(void)
{
	_registration_flags = 0;
	_mode.clear();
	_nick.clear();
	_user.clear();
}


bool client_t::operator==(client_t &rhs) const
{
	return (this->port == rhs.port && this->addr == rhs.addr);
}

bool client_t::operator==(const client_t &rhs) const
{
	return (this->port == rhs.port && this->addr == rhs.addr);
}

/*
** Mode functions
*/


bool	valid_oper_credentials(std::string username, server_t &server, client_t &client)
{
	return (client.get_user() == username && server.find_user(username) && server.valid_oper_host(client));
}

std::string user_mode_bitmask(int m)
{
	std::string	mode;
	char w = ('w' * (bool)(m & 4));
	char i = ('i' * (bool)(m & 8));


	if (w)
		mode += "w";
	if (i)
		mode += "i";
	else
		mode = "empty";
	return mode;
}
