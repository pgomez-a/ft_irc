#include "Command.hpp"

Command::Command(void) : _command_name("DEFAULT") {};

Command::~Command(void) {};

void	Command::who_am_i(void)
{
	std::cout << "I am a humble " << _command_name << std::endl;
}