#ifndef MODE_HPP
# define MODE_HPP
#include "commands.hpp"

class Mode	: public Command
{
	public : 

		Mode(void);

		Channel	*get_channel(void);
	private:

		int	_effect(server_t &server, client_t &client);
		int _channel_mode(server_t &server, client_t &client);
		int _user_mode(server_t &server, client_t &client);

		server_t::channel_map::iterator	_channel_iterator;
};
#endif
