#include "Command.hpp"
#include "reply_system.hpp"

Command::Command(void) : rest_sent(false), _command_name("DEFAULT"), _id(0), _argc(0) {};

Command::~Command(void) 
{
	_clear_argt();
};

void	Command::_clear_argt(void)
{
	if (_argc)
	{
		delete [] _argt;
		_argc = 0;
	}
}

void	Command::who_am_i(void) const
{
	std::cout << _command_name << std::endl;
}

std::string	Command::get_name(void) const
{
	return _command_name;
}

size_t	Command::id(void) const
{
	return _id;
}

static bool	registration_request(int command_id)
{
	return (command_id >= 0 && command_id <= 3);
}

int Command::execute(server_t &server, client_t &client)
{
	int	reply_code;

	if (client.registered() || registration_request(_id))
	{
		reply_code = _effect(server, client);
		if (reply_code == -1)
			return -1;
	}
	else
		reply_code = ERR_NOTREGISTERED;
	return reply_to_client(reply_code, client, server, this);
}


std::string Command::get_aux_msg(void) const
{
	return "something went wrong...";
}
void		Command::set_aux_msg(std::string m)
{
	(void)m;
}

int	Command::_effect(server_t &server, client_t &client)
{
	(void)server, (void)client;
	std::cerr << "parent Command _effect() called" << std::endl;
	return 0;
}

void	Command::set_members(std::string *argt, size_t argc, std::string origin, bool rest_s, std::string rest, size_t error)
{
	_clear_argt();
	if (argc)
		_argt = new std::string[argc];
	for (size_t i = 0; i < argc; ++i)
	{
		_argt[i] = argt[i];
	}
	_argc = argc;
	_origin = origin;
	rest_sent = rest_s;
	_rest = rest;
	_error = error;
}

void	Command::set_name(std::string n)
{
	_command_name = n;
}

std::string	Command::get_rest(void) const
{
	return _rest;
}

Channel	*Command::get_channel(void)
{
	std::cerr << "parent Command get_channel() called" << std::endl;
	return 0;
}

std::string	Command::operator[](size_t i) const
{
	return _argt[i];
}
