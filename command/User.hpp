#ifndef USER_HPP
# define USER_HPP
#include "commands.hpp"

class User : public Command
{
	public : 

		User(void);
		
	private :
		
		int _effect(server_t &server, client_t &client);
};
#endif
