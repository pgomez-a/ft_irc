#include "Command.hpp"

Command::Command(void) : _command_name("DEFAULT") {};

Command::~Command(void) {};

void	Command::who_am_i(void)
{
	std::cout << _command_name << std::endl;
}

std::string	Command::get_name(void)
{
	return _command_name;
}

void Command::parentheses(std::string origin, size_t argc, std::string *argt, std::string rest)
{
	std::cout << "\t\tOrigin:	" <<  origin << std::endl;
	std::cout << "\t\tArguments:	";
	for (size_t i = 0;  i < argc; ++i)
	{
		std::cout << argt[i];
		if ((i+1) < argc)
			std::cout << ", ";
	}
	std::cout << std::endl;
	std::cout << "\t\tMessages:	" << rest << std::endl;
}