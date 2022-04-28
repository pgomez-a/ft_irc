#include "test.hpp"
#include <stdio.h>
#include "../parser/lexer.hpp"
#include "../parser/parser.hpp"

# define RED_BG "\x1b[41m"
# define RESET_COLOR "\x1b[0m"

std::string	eval_token(token_type &x)
{
	std::string	eval;

	if( x.type == PREFIX)
		eval = "PREFIX";
	else
	{
		eval = "WORD(" + x.content + ")";
	}
	return (eval);
}

void	put_token_list(std::string input, std::string expected, token_list l)
{
	std::string	actual;

	expected = " " + expected;
	for (token_list::iterator i = l.begin(); i != l.end(); ++i)
	{
		actual += " " + eval_token(*i);
	}
	std::string validation = (actual == expected) ? "✅" : "❌";
	printf("\t[%s]", validation.c_str());
	printf("input := '%s'\n", input.c_str());
	if (validation == "❌")
		printf("\t\t%sBAD_TOKEN_ERR:%s actual : %s expected : %s\n", RED_BG, RESET_COLOR, actual.c_str(), expected.c_str());
	std::cout << std::endl;
}

int main(void)
{
	printf("MESSAGE PARSING TESTS\n\n");
	{
		token_list  l;
		std::string input;
		
		printf("lexer:\n");

		printf("\tbasic input:\n\n");
		input = "NICK amy";
		l = message_lexer(input);
		put_token_list(input, "WORD(NICK) WORD(amy)", l);

		input = "MODE amy +o";
		l = message_lexer(input);
		put_token_list(input, "WORD(MODE) WORD(amy) WORD(+o)", l);
		
		input = "QUIT";
		l = message_lexer(input);
		put_token_list(input, "WORD(QUIT)", l);

	}

	return (0);
}