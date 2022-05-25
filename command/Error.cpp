#include "Error.hpp"

Error::Error(void) 
{
	_command_name = "ERROR";
	_id = ERROR;
};

void	Error::execute(server_t &server, client_t &client)
{
	std::cout << "THIS IS AN ERROR ()\n";
	(void)server, (void)client;
}