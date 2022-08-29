#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <list>
# include <iostream>
# include "server.hpp"
# include "word_composition.hpp"


class Command
{
	public :
	
		Command();
		~Command(void);
	
		virtual int	execute(server_t &server, client_t &client);
		virtual	std::string	get_aux_msg(void) const;
		virtual	void		set_aux_msg(std::string m);

		void		who_am_i(void) const;
		std::string	get_name(void) const;
		std::string	get_rest(void) const;

		virtual	Channel	*get_channel(void);

		size_t		id(void) const;
		void		set_members(std::string *argt, size_t argc, std::string origin, bool rest_s, std::string rest, size_t error);
		void		set_name(std::string n);

		std::string	operator[](size_t i) const;

		bool		rest_sent;
		
	protected :

		virtual	int				_effect(server_t &server, client_t &client);
		void					_clear_argt(void);

		std::string		_command_name;
		size_t			_id;
		std::string		*_argt;
		size_t			_argc;
		std::string		_origin;
		std::string		_rest;
		size_t			_error;
};

#endif
