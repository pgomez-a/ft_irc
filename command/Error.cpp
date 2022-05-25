#include "Error.hpp"

Error::Error(void) {
	_command_name = "ERROR";
	_id = ERROR;
};

void	Error::operator()(server_t &server, client_t &client)
{
	std::cout << "error ()\n";
	(void)server, (void)client;
}