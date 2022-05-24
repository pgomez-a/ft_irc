#include "lexer.hpp"

/**
 ** get_token - Creates a token (TOKEN_TYPE, word)
 **/

static void	get_token(const std::string	&word, token_type	&token)
{
	static size_t	types[2] = {PREFIX, WORD};
	int				index;

	index = (word != ":");
	token.type = types[index];
	token.content = word;
	return ;
}

/**
 ** get_lexer - Tokenizes the received input
 **/

token_list	message_lexer(const std::string &input)
{
	token_list		list;
	token_type		token;
	std::string		word;
	
	size_t			i = 0, l = input.length();
	while (i < l)
	{
		while (input[i] && !(input[i] == ' ' || word == ":"))
			word += input[i++];
		if (word.size() > 0)
		{
			get_token(word, token);
			list.push_back(token);
			word.clear();
			if (token.type == PREFIX && list.size() > 1 && input[i])
			{
				get_token(std::string(input.begin() + i, input.end()) , token);
				list.push_back(token);
				return (list);
			}
		}
		i += input[i] == ' ';
	}
	return (list);
}