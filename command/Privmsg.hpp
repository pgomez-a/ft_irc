#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP
#include "commands.hpp"

class Privmsg : public Command
{
	public : 

		Privmsg(void);

	private :

		int _effect(server_t &server, client_t &client);
};

#endif
