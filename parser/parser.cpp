#include "parser.hpp"
#include "commands.hpp"

/* DATA STRUCTURES */
static std::map<std::string, Command *>	command_map;

static	rule	rule_matrix[5][5] = {	
									{
										rule1_prefixed_message_expansion,
										NULL,
										NULL,
										rule7_discard_symbol,
										rule6_rest_prefix
									},
									{
										rule2_message_expansion,
										rule4_command_expansion,
										rule3_origin_expansion,
										rule5_parameters_expansion,
										rule8_rest_expansion
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

bool	valid_hostaddr(std::string s) //ip4 addresses only
{
	std::string::iterator	i = s.begin();
	size_t					p = 0;
	size_t					d = 0;

	while (i != s.end())
	{
		if (digit(*i) && d <= 3)
			++d;
		else if (*i == '.' && (p < 4 && d >= 1))
		{
			++p;
			d = 0;
		}
		else
			return false;
		++i;
	}
	return true;
}

bool valid_host(std::string s)
{
	return (valid_hostaddr(s) || valid_servername(s));
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

bool	valid_user_origin(std::string n)
{
	size_t		p;
	std::string	m; //nickname
	std::string	u; //user
	std::string	h; //host

	p = n.find("@");
	if (p != std::string::npos)
		h = n.substr(p, n.length() - p);
	p = n.find("!");
	if (p != std::string::npos)
		u = n.substr(p, n.length() - (p + h.length()));
	m = n.substr(0, n.length() - u.length() - h.length());
	if ((!h.empty() && !valid_host(m)) || ((!u.empty() && !valid_user(u))))
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

Command	*valid_command(token_type &t)
{
	Command	*c = NULL;
	std::string s = "UNKNOWN_COMMAND";

	if (check_str(t.content, letter) || (t.content.size() == 3 && check_str(t.content, digit)))
		s = t.content;
	c = (command_map.find(s))->second;
	return c;
}

/////////////////////////////////////////

static std::string rest(token_type &t)
{	
	std::string	r;

	if (valid_rest(t.content))
		r = t.content;
	return r;
}

static std::string parameter(token_type &t)
{
	std::string	p;

	if (valid_parameter(t.content))
		p = t.content;
	return p;
}

static std::string	origin(token_type &t)
{
	std::string	o;

	if (valid_servername(t.content)|| valid_user_origin(t.content))
		o = t.content;
	return o;
}

/////////////////////////////////////////

void	rule1_prefixed_message_expansion(symbol_stack &s, parser_product &p, token_type &t)
{
	s.empty();
	s.push(I);
	s.push(ORIGIN);
	s.push(PREFIX);

	(void)p;
	(void)t;
}

void	rule2_message_expansion(symbol_stack &s, parser_product &p, token_type &t)
{
	s.pop();
	s.push(REST);
	s.push(PARAMETER);
	s.push(C);

	(void)p;
	(void)t;
}

void	rule3_origin_expansion(symbol_stack &s, parser_product &p, token_type &t)
{
	t.type = ORIGIN;
	p.origin = origin(t);
	(void)s;
}

void	rule4_command_expansion(symbol_stack &s, parser_product &p, token_type &t)
{
	t.type = C;
	p.command = valid_command(t);
	(void)s;
}

void	rule5_parameters_expansion(symbol_stack &s, parser_product &p, token_type &t)
{
	if (p.argc < 14)
	{
		p.argt[p.argc] = parameter(t);
		if (!p.argt[p.argc].empty())
		{
			t.type = PARAMETER;
			s.push(PARAMETER);
			p.argc++;
		}
		else
			p.error = BAD_PARAMETER;
	}
	else
		p.error = TOO_MANY_PARAMETERS;
}

void	rule6_rest_prefix(symbol_stack &s, parser_product &p, token_type &t)
{
	t.type = REST;
	s.push(REST);

	(void)p;
}

void	rule7_discard_symbol(symbol_stack &s, parser_product &p, token_type &t)
{
	s.pop();
	(void)p;
	(void)t;
}

void	rule8_rest_expansion(symbol_stack &s, parser_product &p, token_type &t)
{
	t.type = REST;
	p.rest = rest(t);
	(void)s;
}
/////////////////////////////////////////


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
