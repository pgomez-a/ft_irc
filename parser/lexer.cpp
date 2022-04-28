#include "lexer.hpp"

/**
 ** get_token - Creates a token (TOKEN_TYPE, TOKEN_WORD)
 **/

static void	get_token(const std::string& token_word, std::pair<std::string, std::string>& token_pair)
{
	int			index;
	std::string		token_type[3] = {"PREFIX", "PARAMS", "COMMAND"};
	std::set<std::string>	irc_commands;

	irc_commands.insert("NICK");
	irc_commands.insert("USER");
	irc_commands.insert("QUIT");

	index = (token_word[0] == ':') + (irc_commands.count(token_word)) + (token_word[0] != ':');
	index *= (token_word[0] != ':');
	token_pair.first = token_type[index];
	token_pair.second = token_word;
	return ;
}

/**
 ** get_lexer - Tokenizes the received input
 **/

void		get_lexer(const std::string& input, std::vector<std::pair<std:: string, std::string> >& token_list)
{
	int					count;
	std::string				token_word;
	std::pair<std::string, std::string>	token_pair;

	count = 0;
	while (input[count])
	{
		if (input[count] != ' ' || (token_word[0] == ':' && token_list.size() > 0))
			token_word += input[count];
		else if (token_word.size() > 0)
		{
			get_token(token_word, token_pair);
			token_list.push_back(token_pair);
			token_word.clear();
		}
		count++;
	}
	if (token_word.size() > 0)
	{
		get_token(token_word, token_pair);
		token_list.push_back(token_pair);
		token_word.clear();
	}
	return ;
}