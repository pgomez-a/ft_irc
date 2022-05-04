#ifndef PARSER_HPP
# define PARSER_HPP

# include "lexer.hpp"
# include "commands.hpp"

typedef	std::list<Command *>	command_list;

command_list	message_parser(token_list	tokens);

#endif