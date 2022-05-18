#include "parser.hpp"
#include "commands.hpp"

/* DATA STRUCTURES */
static std::map<std::string, Command *>	command_map;

static	rule	rule_matrix[5][5] = {	
									{
										rule1_prefixed_message_expansion,
										NULL,
										NULL,
										NULL,
										NULL//rule6_rest_expansion
									},
									{
										rule2_message_expansion,
										rule4_command_expansion,
										rule3_origin_expansion,
										NULL,
										NULL
									}
									};


/* UTILS */

static void	init_command_map(void)
{
	static	Command command_array[] = {Nick(), User()};

	command_map.insert(std::make_pair("NICK", &command_array[NICK]));
	command_map.insert(std::make_pair("USER", &command_array[USER]));
	command_map.insert(std::make_pair("UNKNOWN_COMMAND", &command_array[UNKNOWN_COMMAND]));
}

/* SYMBOL EXPANSION AND VALIDITY*/

bool	valid_shortname(std::string s)
{
	bool p;
	
	p = (letter(s[0]) || digit(s[0]));
	if (p)
	{
		for (std::string::iterator	i = s.begin(); (i != s.end() && p); ++i)
		{
			if (!(letter(*i) || digit(*i) || c(*i, '-')) || distance(s.begin(), i) > 64)
				return false;
		}
	}
	return p;
}

bool	valid_nickname(std::string s)
{
	bool p;

	p = (letter(s[0]) || special(s[0]));

	return p;
}

bool valid_user(std::string s)
{
	return check_str(s, user);
}

bool valid_host(std::string s)
{
	return check_str(s, host);
}

bool	servername(std::string n)
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
		i = n.find(".", i + s.length());
		s = n;
		if (i < s.length())
		{
			s = n.substr(j, i);
			++i;
		}
		if (!valid_shortname(s))
			return false;
	}
	return true;
}

bool	user_origin(std::string	n)
{
	size_t		p;
	std::string	m; //nickname
	std::string	u; //user
	std::string	h; //host

	p = n.find("@");
	if (p != std::string::npos)
	{
		h = n.substr(p, n.length() - p);
	}
	p = n.find("!");
	if (p != std::string::npos)
	{
		u = n.substr(p, n.length() - (p + h.length()));
	}
	m = n.substr(0, n.length() - u.length() - h.length());
	if ((!h.empty() && !valid_host(m)) || ((!u.empty() && !valid_user(u))))
		return (false);
	return true;
}

static std::string	origin(token_type &t)
{
	std::string	o;

	if (servername(t.content)|| user_origin(t.content))
		o = t.content;
	return o;
}

static Command	*valid_command(token_type &t)
{
	Command	*c = NULL;
	std::string s = "UNKNOWN_COMMAND";

	if (check_str(t.content, letter) || (t.content.size() == 3 && check_str(t.content, digit)))
		s = t.content;
	c = (command_map.find(s))->second;
	return c;
}

void	rule1_prefixed_message_expansion(symbol_stack &s, parser_product &p, token_type &t)
{
	//std::cout << "PREFIX ORIGIN I\n";
	s.empty();
	s.push(I);
	s.push(ORIGIN);
	s.push(PREFIX);

	(void)p;
	(void)t;
}

void	rule2_message_expansion(symbol_stack &s, parser_product &p, token_type &t)
{
	//std::cout << "C PARAMETERS [PREFIX REST]\n";
	s.pop();
	s.push(REST);
	s.push(PARAMETERS);
	s.push(C);

	(void)p;
	(void)t;
}

void	rule3_origin_expansion(symbol_stack &s, parser_product &p, token_type &t)
{
	//std::cout << "origin x\n";
	t.type = ORIGIN;
	p.origin = origin(t);
	(void)s;
	(void)p;
	(void)t;
}



void	rule4_command_expansion(symbol_stack &s, parser_product &p, token_type &t)
{
	
	p.command = valid_command(t);
	s.pop();
}

static parser_product	parsing_loop(token_list &l, symbol_stack &s)
{
	token_list::iterator	t = l.begin();
	token_list::iterator	u = l.end();
	parser_product			p = {};
	size_t					c = 0; // current symbol 
	rule					r = 0;

	while (t != u && !p.error)
	{	
		c = s.top();
		r = rule_matrix[t->type][c];
		if (r)
			r(s, p, *t);
		if (c == t->type)
		{
			s.pop();
			++t;
		}
	}
	return p;
}

parser_product	message_parser(token_list &tokens)
{
	symbol_stack			symbol;

	if (command_map.empty())
		init_command_map();
	symbol.push(I);

	return parsing_loop(tokens, symbol);
}
