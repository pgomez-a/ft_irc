#ifndef QUIT_HPP
# define QUIT_HPP
#include "commands.hpp"

class Quit : public Command
{
	public : 

		Quit(void);

	private :

		int _effect(server_t &server, client_t &client);
};
#endif
