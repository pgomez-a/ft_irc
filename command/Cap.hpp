#ifndef CAP_HPP
# define CAP_HPP
#include "commands.hpp"

class Cap : public Command
{
	public : 

		Cap(void);

	private :

		int	_effect(server_t &server, client_t &client);

};
#endif
