#include "ircserv.hpp"
#include "commands.hpp"

/**
 ** Parses the message sent by the client for later execution.
 **/

static Command	*process_message(char *buf, size_t buf_len)
{
	std::string		message(buf, 0, buf_len - 1);
	token_list		l;
	parser_product	p;

	l = message_lexer(message);
	p = message_parser(l);
//	if ((--l.end())->type == REST)
//	{
//		p.rest_sent = true;
//	}
	p.produce_command();

	return p.command;
}

/**
 ** Executes the request made by the client.
 **/

static int		executor(char *buf, size_t buf_len, server_t &server, client_t &client)
{
	Command *c;
	int 	r;

	c = process_message(buf, buf_len);
	r = c->execute(server, client);
	return r;
}

/**
 ** Splits the request sent by the client to execute one command at a time.
 **/

int				execute_client_request(char *buf, size_t buf_len, server_t &server, client_t &client)
{
	int	request_len;
	int	request_split;
	int	execute_code;

	request_len = 0;
	request_split = 0;
	while (buf[request_len])
	{
		if (request_len > 0 && buf[request_len] == '\n' && buf[request_len - 1] == '\r')
		{
			buf[request_len - 1] = '\0';
			report_event(event_format(client.addr, client.port, buf + request_split));
			execute_code = executor(buf + request_split, buf_len - 1, server, client);
			if (execute_code == -1)
				return -1;
			if (execute_code == -2)
		 		return on_error("send()", -2);
			request_split = request_len + 1;
		}
		++request_len;
	}
	return 0;
}
