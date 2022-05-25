#include "Command.hpp"

Command::Command(void) : _command_name("DEFAULT"), _id(0){};

Command::~Command(void) {};

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

void Command::execute(server_t &server, client_t &client)
{
	std::cout << "generic () :" << _id;
	who_am_i();
	_reply(server, client, "A reply goes here\n");
}

void	Command::_reply(server_t &server, client_t &client, std::string reply)
{
	std::cout << reply;
	(void)server, (void)client;
}

void	Command::set_members(std::string *argt, size_t argc, std::string origin, std::string rest, size_t error)
{
	_argt = argt;
	_argc = argc;
	_origin = origin;
	_rest = rest;
	_error = error;
}
