#include "Error.hpp"

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
	r = reply_to_client(_error, client, server, this); 
	_command_name = "ERROR";
	return r;
}