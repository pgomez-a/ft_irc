#ifndef TOPIC_HPP
# define TOPIC_HPP
#include "commands.hpp"

class Topic : public Command
{
	public : 

		Topic(void);
	
		Channel	*get_channel(void);

	private :

		int	_effect(server_t &server, client_t &client);
	
		server_t::channel_map::iterator	_channel_iterator;
};
#endif
