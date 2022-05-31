#include "test.hpp"
#include <stdio.h>
#include <cstring>
#include "lexer.hpp"
#include "parser.hpp"

# define RED_BG "\x1b[41m"

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

static std::string		expected_parser_output_format(std::string who_am_i, std::string origin, std::string	arguments, std::string message, int error = 0)
{
	std::string	expected;

	expected += "\n\t\tCommands:	" + who_am_i + "\n";
	expected += "\t\tOrigin:	" + origin + "\n";
	expected += "\t\tArguments:	" + arguments + "\n";
	expected += "\t\tMessages:	" + message + "\n";
	if (error)
	{
		std::stringstream s;
		s << error;
		expected += "\t\tError:\t" + s.str() + "\n";
	}

	return expected;
}

static void	test_parser_product(std::string input, std::string expected, parser_product p)
{
	std::stringstream	captured_output;
	std::streambuf		*original_cout = std::cout.rdbuf();

	std::cout.rdbuf(captured_output.rdbuf());
	std::cout << "\n\t\tCommands:	";
	if (p.command)
	{
		p.command->who_am_i();
	}
	else
	std::cout << "\n";
	std::cout << "\t\tOrigin:	" <<  p.origin << std::endl;
	std::cout << "\t\tArguments:	";
	for (size_t i = 0;  i < p.argc; ++i)
	{
		std::cout << p.argt[i];
		if ((i+1) < p.argc)
			std::cout << ", ";
	}
	std::cout << std::endl;
	std::cout << "\t\tMessages:	" << p.rest << std::endl;
	if (p.error)
	{
		std::cout << "\t\tError:\t" << p.error << "\n";
	}
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

		printf("\n\torigin and command:\n\n");
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

		input = ":ronin!jomon@samur.ai USER";
		expected = expected_parser_output_format("USER", "ronin!jomon@samur.ai", "", "");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = ":n@123.456.789.101 NICK";
		expected = expected_parser_output_format("NICK", "n@123.456.789.101", "", "");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);
		
		printf("\n\tparameters:\n\n");
		
		input = "NICK one";
		expected = expected_parser_output_format("NICK", "", "one", "");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = "NICK one two";
		expected = expected_parser_output_format("NICK", "", "one, two", "");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = "NICK one two three";
		expected = expected_parser_output_format("NICK", "", "one, two, three", "");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = ":123.this.is-a.host NICK one two three four";
		expected = expected_parser_output_format("NICK", "123.this.is-a.host", "one, two, three, four", "");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		printf("\n\trest:\n\n");

		input = "NICK :npinto-g";
		expected = expected_parser_output_format("NICK", "", "", "npinto-g");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = "USER one two three :la la la la lalallalal ala";
		expected = expected_parser_output_format("USER", "", "one, two, three", "la la la la lalallalal ala");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = ":123.this.is-a.host NICK one two three four :this is the rest";
		expected = expected_parser_output_format("NICK", "123.this.is-a.host", "one, two, three, four", "this is the rest");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		printf("\n\t Parser error management:\n\n");

		input = "BADCMD :npinto-g";
		expected = expected_parser_output_format("ERROR", "", "", "", ERR_UNKNOWNCOMMAND);
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);
	
		input = "BADCMD :npinto-g";
		expected = expected_parser_output_format("ERROR", "", "", "", ERR_UNKNOWNCOMMAND);
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = ":-badservername USER :npinto-g";
		expected = expected_parser_output_format("ERROR", "", "", "", BAD_ORIGIN);
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = ":badnicknameistoolong@123.456.789.101.100 NICK";
		expected = expected_parser_output_format("ERROR", "", "", "", BAD_ORIGIN);
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = ":badipv4@123.456.789.1001 NICK";//bad ip but good servername!
		expected = expected_parser_output_format("NICK", "badipv4@123.456.789.1001", "", "");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = ":badipv4@1023.456.789.101 NICK";//bad ip but good servername!
		expected = expected_parser_output_format("NICK", "badipv4@1023.456.789.101", "", "");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = ":badipv4@123.4560.789.101 NICK";//bad ip but good servername!
		expected = expected_parser_output_format("NICK", "badipv4@123.4560.789.101", "", "");
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = ":toolong@123.4560.789.101111111111111111111111111111111111111111111111111111111 NICK";//bad ip and oversized servername
		expected = expected_parser_output_format("ERROR", "", "", "", BAD_ORIGIN);
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = ":@missingnickname NICK";
		expected = expected_parser_output_format("ERROR", "", "", "", BAD_ORIGIN);
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = ":nick!@missing.user.after.exclamation USER";
		expected = expected_parser_output_format("ERROR", "", "", "", BAD_ORIGIN);
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = ":missing!host@";
		expected = expected_parser_output_format("ERROR", "", "", "", BAD_ORIGIN);
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = "NICK one two too many args arg arr args a a a a a a a a a a";
		expected = expected_parser_output_format("ERROR", "", "", "", TOO_MANY_PARAMETERS);
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = "NICK this parameter \n\r is wrong";
		expected = expected_parser_output_format("ERROR", "", "", "", BAD_PARAMETER);
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = "NICK bad rest coming up : \n\r ye";
		expected = expected_parser_output_format("ERROR", "", "", "", BAD_TRAILING);
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

		input = "";
		expected = expected_parser_output_format("ERROR", "", "", "", NO_TOKENS);
		tokens = message_lexer(input);
		p = message_parser(tokens);
		test_parser_product(input, expected,p);

	}
	return (0);
}