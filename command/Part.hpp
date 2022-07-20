#ifndef PART_HPP
# define PART_HPP
#include "commands.hpp"

class Part : public Command
{
	public : 

		Part(void);

	private :

		int	_effect(server_t &server, client_t &client);

		server_t::channel_map::iterator	_channel_iterator;
};

#endif
