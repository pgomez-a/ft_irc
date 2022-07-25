#ifndef LEXER_HPP
# define LEXER_HPP

# include <iostream>
# include <string>
# include <list>

/**
 ** lexer.cpp
 **/

enum	e_token_types{PREFIX, WORD};

typedef	struct	s_token
{
	std::string	content;
	size_t		type;
}				token_type;

typedef std::list<token_type>	token_list; 

token_list  message_lexer(const std::string &input);

#endif

