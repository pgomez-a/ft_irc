#ifndef	JOIN_HPP
# define JOIN_HPP

# include "commands.hpp"

class Join : public Command
{
	public : 

		Join(void);


		Channel	*get_channel(void);
		

	private :

		int	_effect(server_t &server, client_t &client);
		
		server_t::channel_map::iterator	_channel_iterator;
};
#endif
