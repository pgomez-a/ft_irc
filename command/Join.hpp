#ifndef	JOIN_HPP
# define JOIN_HPP

# include "commands.hpp"

class Join : public Command
{
	public : 

		Join(void);

	private :

		int	_effect(server_t &server, client_t &client);
};
#endif
