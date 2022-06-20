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

typedef	struct	s_parser_product 
{
	Command		*command;
	std::string	argt[14];
	size_t		argc;
	std::string	origin;
	std::string	rest;
	size_t		error;

	Command		*produce_command(void);
}				parser_product;
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
