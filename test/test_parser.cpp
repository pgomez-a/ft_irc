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

	if (l.size())
		expected = " " + expected;
	for (token_list::iterator i = l.begin(); i != l.end(); ++i)
	{
		actual += " " + eval_token(*i);
	}
	std::string validation = (actual == expected) ? "✅" : "❌";
	printf("\t[%s]", validation.c_str());
	printf("input := '%s'\n", input.c_str());
	if (validation == "❌")
		printf("\t\t%sBAD_TOKEN_ERR:%s actual : {%s} expected : {%s}\n", RED_BG, RESET_COLOR, actual.c_str(), expected.c_str());
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

		input = "";
		l = message_lexer(input);
		put_token_list(input, "", l);

		input = "QUITQUIT' 'QUITQUIT";
		l = message_lexer(input);
		put_token_list(input, "WORD(QUITQUIT') WORD('QUITQUIT)", l);

		printf("\tinput with prefixed remainder:\n\n");

		input = "PRIVMSG rory :Hey Rory...";
		l = message_lexer(input);
		put_token_list(input, "WORD(PRIVMSG) WORD(rory) PREFIX WORD(Hey Rory...)", l);

		input = "PRIVMSG #cmsc23300 :Hello everybody";
		l = message_lexer(input);
		put_token_list(input, "WORD(PRIVMSG) WORD(#cmsc23300) PREFIX WORD(Hello everybody)", l);

		input = "QUIT :Done for the day, leaving";
		l = message_lexer(input);
		put_token_list(input, "WORD(QUIT) PREFIX WORD(Done for the day, leaving)", l);

		input = "QUIT :";
		l = message_lexer(input);
		put_token_list(input, "WORD(QUIT) PREFIX", l);

		printf("\tinput with prefixed origin:\n\n");


		input = ":borja!borja@polaris.cs.uchicago.edu PRIVMSG #cmsc23300 :Hello everybody";
		l = message_lexer(input);
		put_token_list(input, "PREFIX WORD(borja!borja@polaris.cs.uchicago.edu) WORD(PRIVMSG) WORD(#cmsc23300) PREFIX WORD(Hello everybody)", l);

		input = ":doctor!doctor@baz.example.org QUIT :Done for the day, leaving";
		l = message_lexer(input);
		put_token_list(input, "PREFIX WORD(doctor!doctor@baz.example.org) WORD(QUIT) PREFIX WORD(Done for the day, leaving)", l);
		
		input = ":dusty@mesa.org DESPAIR";
		l = message_lexer(input);
		put_token_list(input, "PREFIX WORD(dusty@mesa.org) WORD(DESPAIR)", l);

		input = ":lo@ne.ly";
		l = message_lexer(input);
		put_token_list(input, "PREFIX WORD(lo@ne.ly)", l);

		printf("\tmisc input:\n\n");

		input = ":::::::";
		l = message_lexer(input);
		put_token_list(input, "PREFIX PREFIX WORD(:::::)", l);

		input = "              :                VANZETTI:               :::::";
		l = message_lexer(input);
		put_token_list(input, "PREFIX WORD(VANZETTI:) PREFIX WORD(::::)", l); //Is this the correct expected output?

		input = ":::: MISTAKES ::::";
		l = message_lexer(input);
		put_token_list(input, "PREFIX PREFIX WORD(:: MISTAKES ::::)", l); //Is this the correct expected output?
		
		input = ":NON:SENSE:";
		l = message_lexer(input);
		put_token_list(input, "PREFIX WORD(NON:SENSE:)", l); //Is this the correct expected output?

		input = "              :spaces@space.s        space :spaces";
		l = message_lexer(input);
		put_token_list(input, "PREFIX WORD(spaces@space.s) WORD(space) PREFIX WORD(spaces)", l);
		
		input = "              :   extra@space.s        space : spaces  ";
		l = message_lexer(input);
		put_token_list(input, "PREFIX WORD(extra@space.s) WORD(space) PREFIX WORD( spaces  )", l); //Is this the correct expected output?
	}

	return (0);
}