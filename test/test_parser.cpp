#include "test.hpp"
#include <stdio.h>
#include <cstring>
#include "lexer.hpp"
#include "parser.hpp"

# define RED_BG "\x1b[41m"
# define RESET_COLOR "\x1b[0m"

static std::string	eval_token(token_type &x)
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

static void	output_validation(std::string input, std::string actual, std::string expected, std::string err)
{
	std::string validation = (actual == expected) ? "✅" : "❌";
	printf("\t[%s]", validation.c_str());
	printf("input := '%s'", input.c_str());
	if (validation == "❌")
		printf("\n\t\t%s%s%s actual : {%s} expected : {%s}\n", RED_BG, err.c_str(), RESET_COLOR, actual.c_str(), expected.c_str());
	std::cout << std::endl;
}

static void	test_token_list(std::string input, std::string expected, token_list l)
{
	std::string	actual;

	if (l.size())
		expected = " " + expected;
	for (token_list::iterator i = l.begin(); i != l.end(); ++i)
	{
		actual += " " + eval_token(*i);
	}
	output_validation(input, actual, expected, "BAD_TOKEN");
}

static std::string		expected_parser_output_format(std::string who_am_i, std::string origin, std::string	arguments, std::string message)
{
	std::string	expected;

	expected += "\n\t\tCommands:	" + who_am_i + "\n";
	expected += "\t\tOrigin:	" + origin + "\n";
	expected += "\t\tArguments:	" + arguments + "\n";
	expected += "\t\tMessages:	" + message + "\n";

	return expected;
}

static void	test_parser_product(std::string input, std::string expected, parser_product p)
{
	std::stringstream	captured_output;
	std::streambuf		*original_cout = std::cout.rdbuf();

	std::cout.rdbuf(captured_output.rdbuf());
	std::cout << "\n\t\tCommands:	";
	p.command->who_am_i();
	std::cout << "\t\tOrigin:	" <<  p.origin << std::endl;
	std::cout << "\t\tArguments:	";
	for (size_t i = 0; p.arguments[i].size() && i < 14; ++i)
	{
		std::cout << p.arguments[i] << ", ";
	}
	std::cout << std::endl;
	std::cout << "\t\tMessages:	" << p.rest << std::endl;
	output_validation(input, captured_output.str(), expected, "BAD_CMD_LST");
	std::cout.rdbuf(original_cout);
	std::cout << std::endl;
}

int main(void)
{
	printf("MESSAGE PARSING TESTS\n\n");
	{
		// token extraction testing
		token_list  l;
		std::string input;
		
		printf("lexer:\n");

		printf("\n\tbasic input:\n\n");
		input = "NICK amy";
		l = message_lexer(input);
		test_token_list(input, "WORD(NICK) WORD(amy)", l);

		input = "MODE amy +o";
		l = message_lexer(input);
		test_token_list(input, "WORD(MODE) WORD(amy) WORD(+o)", l);
		
		input = "QUIT";
		l = message_lexer(input);
		test_token_list(input, "WORD(QUIT)", l);

		input = "";
		l = message_lexer(input);
		test_token_list(input, "", l);

		input = "QUITQUIT' 'QUITQUIT";
		l = message_lexer(input);
		test_token_list(input, "WORD(QUITQUIT') WORD('QUITQUIT)", l);

		printf("\n\tinput with prefixed remainder:\n\n");

		input = "PRIVMSG rory :Hey Rory...";
		l = message_lexer(input);
		test_token_list(input, "WORD(PRIVMSG) WORD(rory) PREFIX WORD(Hey Rory...)", l);

		input = "PRIVMSG #cmsc23300 :Hello everybody";
		l = message_lexer(input);
		test_token_list(input, "WORD(PRIVMSG) WORD(#cmsc23300) PREFIX WORD(Hello everybody)", l);

		input = "QUIT :Done for the day, leaving";
		l = message_lexer(input);
		test_token_list(input, "WORD(QUIT) PREFIX WORD(Done for the day, leaving)", l);

		input = "QUIT :";
		l = message_lexer(input);
		test_token_list(input, "WORD(QUIT) PREFIX", l);

		printf("\n\tinput with prefixed origin:\n\n");


		input = ":borja!borja@polaris.cs.uchicago.edu PRIVMSG #cmsc23300 :Hello everybody";
		l = message_lexer(input);
		test_token_list(input, "PREFIX WORD(borja!borja@polaris.cs.uchicago.edu) WORD(PRIVMSG) WORD(#cmsc23300) PREFIX WORD(Hello everybody)", l);

		input = ":doctor!doctor@baz.example.org QUIT :Done for the day, leaving";
		l = message_lexer(input);
		test_token_list(input, "PREFIX WORD(doctor!doctor@baz.example.org) WORD(QUIT) PREFIX WORD(Done for the day, leaving)", l);
		
		input = ":dusty@mesa.org DESPAIR";
		l = message_lexer(input);
		test_token_list(input, "PREFIX WORD(dusty@mesa.org) WORD(DESPAIR)", l);

		input = ":lo@ne.ly";
		l = message_lexer(input);
		test_token_list(input, "PREFIX WORD(lo@ne.ly)", l);

		printf("\n\tmisc input:\n\n");

		input = ":::::::";
		l = message_lexer(input);
		test_token_list(input, "PREFIX PREFIX WORD(:::::)", l);

		input = "              :                VANZETTI:               :::::";
		l = message_lexer(input);
		test_token_list(input, "PREFIX WORD(VANZETTI:) PREFIX WORD(::::)", l); //Is this the correct expected output?

		input = ":::: MISTAKES ::::";
		l = message_lexer(input);
		test_token_list(input, "PREFIX PREFIX WORD(:: MISTAKES ::::)", l); //Is this the correct expected output?
		
		input = ":NON:SENSE:";
		l = message_lexer(input);
		test_token_list(input, "PREFIX WORD(NON:SENSE:)", l); //Is this the correct expected output?

		input = "              :spaces@space.s        space :spaces";
		l = message_lexer(input);
		test_token_list(input, "PREFIX WORD(spaces@space.s) WORD(space) PREFIX WORD(spaces)", l);
		
		input = "              :   extra@space.s        space : spaces  ";
		l = message_lexer(input);
		test_token_list(input, "PREFIX WORD(extra@space.s) WORD(space) PREFIX WORD( spaces  )", l); //Is this the correct expected output?
	}
	{
		token_list  	tokens;
		std::string		input;
		std::string		expected;
		parser_product	p;

		//token parsing and correct command association
		printf("\nparser\n");

		input = "NICK";
		expected = expected_parser_output_format("NICK", "", "", "");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = ":basicservername USER";
		expected = expected_parser_output_format("USER", "basicservername", "", "");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = ":123.this.is-a.host NICK";
		expected = expected_parser_output_format("NICK", "123.this.is-a.host", "", "");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		//aux origin (nickname[["!" user]"@"servername])
		input = ":n!npintog@c.com USER";
		expected = expected_parser_output_format("USER", "n!npintog@c.com", "", "");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);
		
		/*input = "NICK arg1 arg2 arg3";
		expected = expected_parser_output_format("NICK", "", "", "");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);*/
	}
	return (0);
}