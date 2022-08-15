#ifndef MODE_HPP
# define MODE_HPP
#include "commands.hpp"

# define	NO_OF_POSSIBLE_FLAGS 128
# define	FIRST_POS_FLAG 65
enum	e_flag_change_type{DO_NOTHING = 0, ADD_FLAG = '+', DROP_FLAG  = '-'};

struct	mode_change
{
	int						change_type;
	std::list<std::string>	arg_list;
};

typedef std::list<mode_change> mode_parse_product;

class Mode	: public Command
{
	public : 

		Mode(void);

		Channel	*get_channel(void);

	private:

		int					_effect(server_t &server, client_t &client);
		int 				_channel_mode(server_t &server, client_t &client);
		int 				_user_mode(client_t &client);
		mode_parse_product	_channel_mode_parser(std::string input);	
		mode_parse_product	_user_mode_parser(std::string input);
		void				_reset_flag_table(void);
		int					_change_user_modes(client_t &client);
		
		server_t::channel_map::iterator	_channel_iterator;
		mode_change						_flag_change_table[NO_OF_POSSIBLE_FLAGS];

};
#endif
