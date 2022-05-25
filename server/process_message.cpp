#include "ircserv.hpp"
#include "commands.hpp"

static void	execute_command(Command *command, server_t &server, client_t &client)
{
	(*command)(server, client);
}

void		process_message(char *buf, size_t buf_len, server_t &server, client_t &client)
{
	std::string	message(buf, 0, buf_len);
	token_list	l;
	Command		*command;

	l = message_lexer(message);
	command = message_parser(l);
	execute_command(command, server, client);
}