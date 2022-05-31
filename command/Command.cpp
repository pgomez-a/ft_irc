#include "Command.hpp"

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

void		Command::set_aux(std::string s)
{
	_aux = s;
}

int Command::execute(server_t &server, client_t &client)
{
	std::string	reply;

	reply = _effect(server, client);
	return reply_to_client(client, reply);
}

std::string	Command::_effect(server_t &server, client_t &client)
{
	return reply_format(server.name, "999", client.addr, "This is " + _command_name + "'s dummy reply...");
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