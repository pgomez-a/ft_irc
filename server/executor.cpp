#include "ircserv.hpp"
#include "commands.hpp"

static Command	*process_message(char *buf, size_t buf_len)
{
	std::string		message(buf, 0, buf_len - 1);
	token_list		l;
	parser_product	p;

	l = message_lexer(message);
	p = message_parser(l);
	p.produce_command();
	return p.command;
}

int				executor(char *buf, size_t buf_len, server_t &server, client_t &client)
{
	Command *c;
	int 	r;

	c = process_message(buf, buf_len);
	r = c->execute(server, client);
	return r;
}
