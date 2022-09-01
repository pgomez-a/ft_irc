#include "commands.hpp"

void	add_flag(std::string &modes, char flag)
{
	if (modes == "empty")
		modes.clear();
	modes += flag;
}

void	rm_flag(std::string &modes, char flag)
{
	size_t i = modes.find(flag);

	if (i != std::string::npos)
		modes.erase(i);
}

bool	flag_is_set(std::string modes, char flag)
{
	return (modes.find(flag) != std::string::npos);	
}

bool	is_operator(std::string modes)
{
	return (flag_is_set(modes, 'o') || flag_is_set(modes, 'O'));
}

