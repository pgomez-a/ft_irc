#ifndef MODE_HPP
# define MODE_HPP
#include "commands.hpp"

# define	NO_OF_POSSIBLE_FLAGS 128
# define	FIRST_POS_FLAG 65

enum	e_flag_change_type{ DO_NOTHING = -1, DROP_FLAG  = 0, ADD_FLAG = 1 };
enum	e_user_mode_parser{INVALID_UMODE, VALID_UMODE};

typedef	int	(*change)
(server_t &s, client_t &c, std::string target,  std::list<std::string> arg, int change, Command *n);

struct mode_change
{

	mode_change(char f, int c);

	char					flag;
	int						change_type;
	std::list<std::string>	arg_list;
};

struct	mode_flag
{
	int						change_count;
	change					apply;
};

int	mode_ban_flag
(server_t &s, client_t &c, std::string chan_name, std::list<std::string> argl, int change, Command *n);
		
int	mode_restrict_topic
(server_t &s, client_t &c, std::string chan_name, std::list<std::string> argl, int change, Command *n);

int	mode_restrict_message
(server_t &s, client_t &c, std::string chan_name, std::list<std::string> argl, int change, Command *n);

int	chan_oper_flag
(server_t &s, client_t &c, std::string chan_name, std::list<std::string> argl, int change, Command *n);

int	local_oper_flag
(server_t &s, client_t &c, std::string user_name, std::list<std::string> argl, int change, Command *n);

int	invisible_flag
(server_t &s, client_t &c, std::string user_name, std::list<std::string> argl, int change, Command *n);

int	wall_flag
(server_t &s, client_t &c, std::string user_name, std::list<std::string> argl, int change, Command *n);


typedef  std::list<mode_change>	parsed_instructions;

class Mode	: public Command
{
	public : 

		Mode(void);

		Channel		*get_channel(void);
		std::string	get_last_mode_request(void);
		std::string	get_aux_msg(void) const;
		void		set_aux_msg(std::string m);
		std::string mode_aux_str;

	private:

		int					_effect(server_t &server, client_t &client);
		int 				_channel_mode(server_t &server, client_t &client);
		int 				_user_mode(server_t &server, client_t &client);
		int					_channel_mode_parser(std::string *input, parsed_instructions &p);	
		void				_extract_cmode_arg(int &i, std::string *input, std::list<std::string> &l);
		int					_apply_channel_modes(server_t &server, client_t &client, parsed_instructions p);
		void				_reset_flag_table(parsed_instructions change, mode_flag *flag_table);
		int					_user_mode_parser(std::string input, parsed_instructions &p);
		int					_apply_user_modes(server_t &server, client_t &client, parsed_instructions &p);

		server_t::channel_map::iterator	_channel_iterator;
		std::string						_last_mode_request;
		std::string						_ban_mask_msg;
		mode_flag						_chan_flag_table[NO_OF_POSSIBLE_FLAGS];
		mode_flag						_user_flag_table[NO_OF_POSSIBLE_FLAGS];

};
#endif
