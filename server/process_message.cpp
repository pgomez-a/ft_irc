#include "ircserv.hpp"
#include "commands.hpp"

static void	execute_command(Command *command, server_t &server, client_t &client)
{
	command->execute(server, client);
}

void		process_message(char *buf, size_t buf_len, server_t &server, client_t &client)
{
	std::string		message(buf, 0, buf_len);
	token_list		l;
	Command			*command;
	parser_product	p;

	l = message_lexer(message);
	p = message_parser(l);
	command = p.command;
	execute_command(command, server, client);
}