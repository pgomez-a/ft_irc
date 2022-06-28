#ifndef NOTICE_HPP
# define NOTICE_HPP
#include "commands.hpp"

class Notice
 : public Command
{
	public : 

		Notice(void);

	private:

		int	_effect(server_t &server, client_t &client);
};
#endif
