#ifndef NICK_HPP
# define NICK_HPP
#include "commands.hpp"

class Nick : public Command
{
	public : 

		Nick(void);

	private :

		std::string		_effect(server_t &server, client_t &client);

};
#endif