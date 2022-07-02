#ifndef	LIST_HPP
# define LIST_HPP

# include "commands.hpp"

class List : public Command
{
	public : 

		List(void);

	private :

		int			_effect(server_t &server, client_t &client);
		std::string	_channel_list_string(server_t &server);	
};
#endif
