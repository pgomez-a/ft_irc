#include "parser.hpp"
#include <ctype.h>

bool	letter(char c){return isalpha(c);}

bool	digit(char c){return isdigit(c);}

bool	user(char c)
{
	std::string	excluded_set(5, 0);

	excluded_set[1] = 13;//cr
	excluded_set[2] = 10;//nl
	excluded_set[3] = ' ';
	excluded_set[4] = '@';
	return !is_in_set(c, excluded_set);
}

bool	rest(char c)
{
	std::string	excluded_set(3, 0);

	excluded_set[1] = 13;//cr
	excluded_set[2] = 10;//nl
	return !is_in_set(c, excluded_set);
}

bool	special(char c)
{
	std::string	special_set("[]\\`-^{|}");

	return is_in_set(c, special_set);
}

bool	c(char c, char d){return (c == d);}


bool	is_in_set(char c, std::string set)
{
	for (size_t i = 0; i < set.length(); ++i)
	{
		if (c == set[i])
			return true;
	}
	return false;
}

bool	check_str(std::string s, char_filter f)
{
	for (size_t i = 0; s[i]; ++i)
	{
		if (!f(s[i]))
			return false;
	}
	return true;
}