#ifndef PING_HPP
# define PING_HPP
#include "commands.hpp"

class Ping : public Command
{
	public : 

		Ping(void);
	
	private :

		int	_effect(server_t &server, client_t &client);
};
#endif
