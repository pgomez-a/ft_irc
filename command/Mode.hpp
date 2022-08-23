#ifndef MODE_HPP
# define MODE_HPP
#include "commands.hpp"

# define	NO_OF_POSSIBLE_FLAGS 128
# define	FIRST_POS_FLAG 65

enum	e_flag_change_type{ DO_NOTHING = -1, DROP_FLAG  = 0, ADD_FLAG = 1 };
enum	e_user_mode_parser{INVALID_UMODE, VALID_UMODE};

struct	mode_change
{
	int						change_type;
	std::list<std::string>	arg_list;
};

typedef std::string parsed_instructions;

class Mode	: public Command
{
	public : 

		Mode(void);

		Channel		*get_channel(void);
		std::string	get_last_mode_request(void);

	private:

		int					_effect(server_t &server, client_t &client);
		int 				_channel_mode(server_t &server, client_t &client);
		int 				_user_mode(client_t &client);
		int					_channel_mode_parser(std::string *input, parsed_instructions &p);	
		void				_extract_cmode_arg(int &i, int j, std::string *input);
		int					_apply_channel_modes(server_t &server, client_t &client, parsed_instructions p);
		void				_reset_flag_table(void);
		int					_user_mode_parser(std::string input, parsed_instructions &p);
		int					_apply_user_modes(client_t &client, parsed_instructions &p);

		server_t::channel_map::iterator	_channel_iterator;
		std::string						_last_mode_request;
		mode_change						_flag_table[NO_OF_POSSIBLE_FLAGS];

};
#endif
