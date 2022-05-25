#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <list>
# include <iostream>
# include "server.hpp"

class Command
{
	public :
	
		Command();
		~Command(void);
	
		virtual void	operator()(server_t &server, client_t &client);
		virtual void	_reply(server_t &server, client_t &client, std::string reply);

		void		who_am_i(void);
		std::string	get_name(void);
		size_t		id(void);
		void		set_members(std::string *argt, size_t argc, std::string origin, std::string rest, size_t error);

	protected :

		std::string		_command_name;
		size_t			_id;
		std::string		*_argt;
		size_t			_argc;
		std::string		_origin;
		std::string		_rest;
		size_t			_error;
};

#endif