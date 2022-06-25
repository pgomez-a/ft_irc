#ifndef OPER_HPP
# define OPER_HPP
#include "commands.hpp"

class Oper : public Command
{
	public : 

		Oper(void);
	
	private :

		int	_effect(server_t &server, client_t &client);
};
#endif
