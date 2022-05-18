#ifndef PARSER_HPP
# define PARSER_HPP

# include "lexer.hpp"
# include "commands.hpp"
# include <stack>
# include <map>

enum	e_PARSER_SYMBOLS
{
	I, //inital parser symbol
	C,
	ORIGIN,
	PARAMETERS,
	REST,
	//ORI
	//
};

typedef	struct	s_parser_product 
{
	Command		*command;
	std::string	arguments[14];
	std::string	origin;
	std::string	rest;
	size_t		error;
}				parser_product;

typedef std::stack<size_t>		symbol_stack; //containing parser and lexer symbols.
typedef std::list<size_t>		symbol_list;

//Grammar rules
typedef	void	(*rule)(symbol_stack &, parser_product &, token_type &);
void			rule1_prefixed_message_expansion(symbol_stack &s, parser_product &p, token_type &t);
void			rule2_message_expansion(symbol_stack &s, parser_product &p, token_type &t);
void			rule3_origin_expansion(symbol_stack &s, parser_product &p, token_type &t);
void			rule4_command_expansion(symbol_stack &s, parser_product &p, token_type &t);

parser_product	message_parser(token_list &tokens);

//Word Composition

typedef	bool (*char_filter)(char);

bool	letter(char c);
bool	digit(char c);
bool	special(char c);
bool	user(char c);
bool	host(char c);
bool	c(char c, char d);

bool	check_str(std::string s, char_filter f);


#endif