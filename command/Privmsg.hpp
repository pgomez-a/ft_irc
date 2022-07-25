#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP
#include "commands.hpp"

class Privmsg : public Command
{
	public : 

		Privmsg(void);
		
		Channel	*get_channel(void);

	private :

		int _effect(server_t &server, client_t &client);

		server_t::channel_map::iterator	_channel_iterator;
};

#endif
