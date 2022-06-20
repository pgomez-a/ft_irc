#ifndef	ERROR_HPP
# define ERROR_HPP
#include "commands.hpp"

class Error : public Command
{
	public : 

		Error(void);

		int			execute(server_t &server, client_t &client);
		void		set_aux(std::string s);
		std::string	get_aux(void);

	private :

};
#endif