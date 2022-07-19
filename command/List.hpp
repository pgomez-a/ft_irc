#ifndef	LIST_HPP
# define LIST_HPP

# include "commands.hpp"

class List : public Command
{
	public : 

		List(void);

		Channel	*get_channel(void);

	private :

		int		_effect(server_t &server, client_t &client);
		void	_channel_list_string(server_t &server, client_t &client);
		void	_channel_list_string(std::string channels, server_t &server, client_t &client);
		

		server_t::channel_map::iterator	_channel_iterator;
};
#endif
