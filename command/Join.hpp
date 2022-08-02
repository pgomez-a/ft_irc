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
		int	_join_loop(server_t &server, client_t &client, std::list<std::string> token_list);
		
		server_t::channel_map::iterator	_channel_iterator;
};
#endif
