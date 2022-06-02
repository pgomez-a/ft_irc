#include "Command.hpp"
#include "reply_system.hpp"

Command::Command(void) : _command_name("DEFAULT"), _id(0), _argc(0) {};

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

void	Command::who_am_i(void)
{
	std::cout << _command_name << std::endl;
}

std::string	Command::get_name(void)
{
	return _command_name;
}

size_t	Command::id(void)
{
	return _id;
}

int Command::execute(server_t &server, client_t &client)
{
	int	reply_code;

	reply_code = _effect(server, client);
	return reply_to_client(reply_code, client, this);
}

int	Command::_effect(server_t &server, client_t &client)
{
	(void)server, (void)client;
	return 0;
}

void	Command::set_members(std::string *argt, size_t argc, std::string origin, std::string rest, size_t error)
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
	_rest = rest;
	_error = error;
}

void	Command::set_name(std::string n)
{
	_command_name = n;
}