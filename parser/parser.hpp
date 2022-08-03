#ifndef PARSER_HPP
# define PARSER_HPP

# include "lexer.hpp"
# include "commands.hpp"
# include "word_composition.hpp"
# include <stack>
# include <map>

# define SERVERNAME_LEN 64

enum	e_PARSER_SYMBOLS
{
	I, //inital parser symbol, parser loops always starts with one
	C,
	ORIGIN,
	PARAMETER,
	REST
	//
	//
};

//Actual error replies  are numbered [400, 599] in RFC 2812
enum	e_PARSER_ERRORS
{
	BAD_PARAMETER = 999,
	TOO_MANY_PARAMETERS = 888,
	BAD_ORIGIN = 777,
	BAD_TRAILING = 10203,
	NO_TOKENS = 10000001
};

struct	parser_product 
{
	parser_product(void);
	parser_product(parser_product const  &p);

	~parser_product(void);

	Command		*command;
	std::string	*argt;
	size_t		argc;
	std::string	origin;
	bool		rest_sent;
	std::string	rest;
	size_t		error;

	Command		*produce_command(void);

	void		operator=(parser_product const &p);
};

void	set_err_product(parser_product &p);

typedef std::stack<size_t>		symbol_stack; //containing parser and lexer symbols.
typedef std::list<size_t>		symbol_list;

//Grammar rules
typedef	void	(*rule)(symbol_stack &, parser_product &, token_type &);
void			rule1_prefixed_message_expansion(symbol_stack &s, parser_product &p, token_type &t);
void			rule2_message_expansion(symbol_stack &s, parser_product &p, token_type &t);
void			rule3_origin_expansion(symbol_stack &s, parser_product &p, token_type &t);
void			rule4_command_expansion(symbol_stack &s, parser_product &p, token_type &t);
void			rule5_parameters_expansion(symbol_stack &s, parser_product &p, token_type &t);
void			rule6_rest_prefix(symbol_stack &s, parser_product &p, token_type &t);
void			rule7_discard_symbol(symbol_stack &s, parser_product &p, token_type &t);
void			rule8_rest_expansion(symbol_stack &s, parser_product &p, token_type &t);


parser_product	message_parser(token_list &tokens);



#endif
