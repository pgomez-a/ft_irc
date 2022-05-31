#include "Error.hpp"

static	std::string	error_421(std::string command_name)
{
	return command_name + " :Unknown command";
}

Error::Error(void) 
{
	_command_name = "ERROR";
	_id = ERROR;
};

int	Error::execute(server_t &server, client_t &client)
{
	std::string reply;
	int			r = 0;

	//do something;
	switch (_error)
	{
		case (421):
			reply = reply_format(server.name, "421", client.addr, error_421(_aux));
			_aux.clear();
			break ;
	}
	if (!reply.empty())
		r = reply_to_client(client, reply);
	_command_name = "ERROR";
	return r;
}