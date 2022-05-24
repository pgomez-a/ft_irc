#include "ircserv.hpp"
#include "commands.hpp"

void	execute_command(parser_product &p)
{
	Error	e;
	Command *ptr = &e;

	if (p.error)
		ptr->parentheses(p.origin, p.argc, p.argt, p.rest);
}
