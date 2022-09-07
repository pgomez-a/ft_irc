#ifndef	NAMES_HPP
# define NAMES_HPP

# include "commands.hpp"

class Names : public Command
{
	public : 

		Names(void);

		Channel	*get_channel(void);

	private :

		int	_effect(server_t &server, client_t &client);
		int	_list_all_channels(server_t &s, client_t &c);	
		int	_list_channel_loop(server_t &server, client_t &client, std::list<std::string> token_list);


		server_t::channel_map::iterator	_channel_iterator;
};
#endif
