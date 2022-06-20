#include "parser.hpp"
#include "word_composition.hpp"
#include "commands.hpp"

/* DATA STRUCTURES */
static std::map<std::string, Command *>	command_map;

static	Command command_array[11] = 
{
Nick(), User(), Error(), Pass(), Oper(), Quit(), Join(), Part(), Privmsg(), Notice(), Ping()
};

static Nick nick_inst; static User user_inst; static Error error_inst; 
static Pass pass_inst; static Oper oper_inst; static Quit quit_inst; 
static Join join_inst; static Part part_inst; static Privmsg privmsg_inst;
static Notice notice_inst; static Ping ping_inst;

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
	command_map.insert(std::make_pair("PING", &ping_inst));

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

void	set_err_product(parser_product &p)
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
