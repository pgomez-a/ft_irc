#include "ircserv.hpp"
#include "commands.hpp"

int			executor(char *buf, size_t buf_len, server_t &server, client_t &client)
{
	Command *c;
	int 	r;

	std::cout << "executor buffer : " << buf << std::endl;
	c = process_message(buf, buf_len);
	r = c->execute(server, client);
	
	return r;
}

Command		*process_message(char *buf, size_t buf_len)
{
	std::string		message(buf, 0, buf_len - 1);
	token_list		l;
	parser_product	p;

	std::cout << "message buffer : " << buf << std::endl;
	l = message_lexer(message);
//	for (token_list::iterator i = l.begin(); i != l.end(); ++i)
	//	std::cout << "tok recv: " << i->content << std::endl;
	p = message_parser(l);
	p.produce_command();
	return p.command;
}
