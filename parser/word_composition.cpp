#include "parser.hpp"
#include <ctype.h>
#include <cstdarg>

bool	letter(char c){return isalpha(c);}

bool	digit(char c){return isdigit(c);}

bool	hyphen(char c){return (c == '-');}

bool	user(char c)
{
	static std::string	excluded_set(5, 0);

	excluded_set[1] = 13;//cr
	excluded_set[2] = 10;//nl
	excluded_set[3] = ' ';
	excluded_set[4] = '@';
	return !is_in_set(c, excluded_set);
}

bool	rest(char c)
{
	static const char excluded_set[3] = {0, 13, 10};

	return !is_in_set(c, excluded_set, 3);
}

bool	parameter(char c)
{
	static const char excluded_set[5] = {0, 13, 10,' ', ':'};

	return !is_in_set(c, excluded_set, 5);
}

bool	special(char c)
{
	static const char	special_set[9] = {'[',']','\\','`','-','^','{','|','}'};

	return is_in_set(c, special_set, 9);
}

bool	c(char c, char d){return (c == d);}

bool	is_in_set(char c, const char *set, size_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		if (c == set[i])
			return true;
	}
	return false;
}

bool	is_in_set(char c, std::string set)
{
	size_t len = set.length();

	for (size_t i = 0; i < len; ++i)
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

bool	check_str(std::string s, std::list<char_filter> l)
{
	bool	p = true;
	
	for (size_t i = 0; s[i] && p; ++i)
	{
		p = false;
		for (std::list<char_filter>::iterator f = l.begin(); f != l.end() && !p; ++f)
		{
			p = (*f)(s[i]);
		}
	}
	return p;
}