#ifndef MODE_HPP
# define MODE_HPP
#include "commands.hpp"

class Mode	: public Command
{
	public : 

		Mode(void);

	private:

		int	_effect(server_t &server, client_t &client);
};
#endif
