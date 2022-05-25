#ifndef	ERROR_HPP
# define ERROR_HPP
#include "commands.hpp"

class Error : public Command
{
	public : 

		Error(void);

		virtual void	operator()(server_t &server, client_t &client);

	private :

};
#endif