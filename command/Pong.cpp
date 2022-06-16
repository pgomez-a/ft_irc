#ifndef PONG_HPP
# define PONG_HPP
#include "commands.hpp"

class Pong : public Command
{
	public : 

		Pong(void);
	
	private :

		int	_effect(server_t &server, client_t &client);
};
#endif
