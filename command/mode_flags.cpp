#include "commands.hpp"

void	add_flag(std::string &modes, char flag)
{
	if (modes == "empty")
		modes.clear();
	modes += flag;
}

bool	flag_is_set(std::string modes, char flag)
{
	return (modes.find(flag) != std::string::npos);	
}

bool	is_operator(std::string modes)
{
	return (flag_is_set(modes, 'o') || flag_is_set(modes, 'O'));
}

