#include "parser.hpp"
#include "word_composition.hpp"
#include <ctype.h>
#include <cstdarg>

bool	letter(char c){return isalpha(c);}

bool	digit(char c){return isdigit(c);}

bool	alphanumeric(char c) {return (letter(c) || digit(c));}

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

bool	chanstring(char c)
{
	static const char excluded_set[7] = {0, 7, 13, 10, ' ', ',', ':'};

	return !is_in_set(c, excluded_set, 7);
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

/* SYMBOL EXPANSION AND VALIDITY*/

bool	valid_shortname(std::string s)
{
	std::list<char_filter> l;
	bool p;
	
	p = (letter(s[0]) || digit(s[0]));
	if (p)
	{
		l.push_back(letter);
		l.push_back(digit);
		l.push_back(hyphen);
		p = check_str(s, l);
	}
	return p;
}

bool	valid_nickname(std::string s)
{
	std::list<char_filter> l;
	bool p;

	p = (letter(s[0]) || special(s[0]));
	if (p == true)
	{
		l.push_back(letter);
		l.push_back(digit);
		l.push_back(hyphen);
		l.push_back(special);
		p = check_str(s, l);
	}
	return (p && s.length() <= 9);
}

bool	valid_channelname(std::string n)
{
	size_t		delimiter;
	std::string	m;
	bool		p;

	m = n;
	p = false;
	std::cout << "bug is below\n";
	std::cout << "n is : " << n << std::endl;
	if (n.size() && (is_in_set(n[0] ,"#+@") || (n[0] == '!' && alphanumeric(n[1]))))
	{
		delimiter = n.find(':');
		n = n.substr(0, delimiter - 1);
		std::cout << "SUBSTR n " << n << std::endl;
		p = check_str(n, chanstring);
		if (delimiter != std::string::npos)
		{
			m = m.substr(delimiter + 1, delimiter + 1 - m.size());
			std::cout << "SUBSTR m " << m << std::endl;
			p = check_str(m, chanstring);
		}
	}
	return p;
}

bool valid_user(std::string s)
{
	return (s.length() && check_str(s, user));
}

bool	valid_hostaddr(std::string s) //ip4 addresses only
{
	std::string::iterator	i = s.begin();
	size_t					p = 0;
	size_t					d = 0;

	while (i != s.end())
	{
		if (digit(*i) &&  (p < 4 && d <= 3))
			++d;
		else if (*i == '.' && (p < 4 && d >= 1 &&  d <= 3))
		{
			++p;
			d = 0;
		}
		else
			return false;
		++i;
	}
	return (p < 4 && d < 4);
}

bool valid_host(std::string s)
{
	return (s.length() && (valid_hostaddr(s) || valid_servername(s)));
}

bool	valid_servername(std::string n)
{
	std::string	s;
	size_t		i;
	size_t		j;
	size_t		l;

	i = 0;
	j = 0;
	l = n.length();
	while (i < l)
	{
		j = i;
		i = n.find(".", i);
		s = n;
		if (i < s.length())
		{
			s = n.substr(j, i - j);
			++i;
		}
		else
		{
					s = n.substr(j, l - j);
		}
		if (!valid_shortname(s))
			return false;
	}
	return (n.length() < SERVERNAME_LEN);
}

bool	valid_user_origin(std::string n)
{
	size_t						x;
	std::pair<bool,std::string>	m; //nickname
	std::pair<bool,std::string>	u; //user
	std::pair<bool,std::string>	h; //host

	x = n.find("@");
	if (x != std::string::npos)
	{	
		h.first = true;
		h.second = n.substr(x + 1, n.length() - x);
	}
	x = n.find("!");
	if (x != std::string::npos)
	{
		u.first = true;
		u.second = n.substr(x + 1, n.length() - (x + h.second.length() + 2));
	}
	//nickname's length :
	//(total length - (user's l + 1*(user exists)) - (host's length + 1*(host exists)))
	x = n.length() - (u.second.length() + u.first) - (h.second.length() + h.first);
	m.second = n.substr(0, x);
	if ((h.first && !valid_host(h.second)) || ((u.first && !valid_user(u.second))) || (!valid_nickname(m.second)))
		return (false);
	return true;
}

bool	valid_rest(std::string r)
{
	bool p = true;

	if (r.length())
	{
		p = check_str(r, rest);
	}
	return p;
}

bool valid_parameter(std::string p)
{
	return check_str(p, parameter);
}


