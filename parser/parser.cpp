#include "parser.hpp"
#include "commands.hpp"

/* DATA STRUCTURES */
static std::map<std::string, Command *>	command_map;

static	Command command_array[10] = 
{
Nick(), User(), Error(), Pass(), Oper(), Quit(), Join(), Part(), Privmsg(), Notice()
};

static Nick nick_inst; static User user_inst; static Error error_inst; static Pass pass_inst; static Oper oper_inst; 
static Quit quit_inst; static Join join_inst; static Part part_inst; static Privmsg privmsg_inst; static Notice notice_inst;

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

	command_map.insert(std::make_pair("NICK", &nick_inst));
	command_map.insert(std::make_pair("USER", &user_inst));
	command_map.insert(std::make_pair("ERROR", &error_inst));
	command_map.insert(std::make_pair("PASS", &pass_inst));
	command_map.insert(std::make_pair("OPER", &oper_inst));
	command_map.insert(std::make_pair("QUIT", &quit_inst));
	command_map.insert(std::make_pair("JOIN", &join_inst));
	command_map.insert(std::make_pair("PART", &part_inst));
	command_map.insert(std::make_pair("PRIVMSG", &privmsg_inst));
	command_map.insert(std::make_pair("NOTICE", &notice_inst));

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

Command	*valid_command(token_type &t)
{
	std::map<std::string, Command *>::iterator	c;
	std::string s = "ERROR";

	if (check_str(t.content, letter) || (t.content.size() == 3 && check_str(t.content, digit)))
		s = t.content;
	c = command_map.find(s);
	if (c  == command_map.end())
		c = command_map.find("ERROR");
	return c->second;
}

/////////////////////////////////////////

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
	if (p.origin.empty())
		p.error = BAD_ORIGIN;
	(void)s;
}

void	rule4_command_expansion(symbol_stack &s, parser_product &p, token_type &t)
{
	t.type = C;
	p.command = valid_command(t);
	if (p.command->id() == ERROR)
	{
		p.error = ERR_UNKNOWNCOMMAND;
		p.command->set_name(t.content);
	}
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
	if (valid_rest(t.content))
		p.rest = t.content;
	else
		p.error = BAD_TRAILING;
	(void)s;
}
/////////////////////////////////////////

Command	*parser_product::produce_command(void)
{
	command->set_members(argt, argc, origin, rest, error);

	return command;
}

static void	set_err_product(parser_product &p)
{
	p.command = &error_inst;
	p.argc = 0;
	p.origin.clear();
	p.rest.clear();
}

static parser_product	parsing_loop(token_list &l, symbol_stack &s)
{
	token_list::iterator	t = l.begin();
	token_list::iterator	u = l.end();
	parser_product			p = {};
	size_t					c = 0; // current symbol 
	rule					r = 0;

	p.error = NO_TOKENS * l.empty();
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
	if (p.error)
		set_err_product(p);
	return p;
}

parser_product	message_parser(token_list &tokens)
{
	symbol_stack			symbol;
	parser_product			p;

	if (command_map.empty())
		init_command_map();
	symbol.push(I);
	p = parsing_loop(tokens, symbol);
	return p;
}
