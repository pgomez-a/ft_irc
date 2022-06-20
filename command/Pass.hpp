#ifndef PASS_HPP
# define PASS_HPP
#include "commands.hpp"

class Pass : public Command
{
	public : 

		Pass(void);
	
	private :

		int	_effect(server_t &server, client_t &client);
};
#endif