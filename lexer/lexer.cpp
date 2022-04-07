#include <iostream>
#include <vector>
#include <set>

static int	get_token(const std::string& token_word, std::pair<std::string, std::string>& token_pair)
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
	return (index);
}

int	main(int argc, char* argv[])
{
	int							mode;
	int							count;
	std::string						token_word;
	std::pair<std::string, std::string>			token_pair;
	std::vector<std::pair<std::string, std::string> >	token_list;

	if (argc != 2)
		return (0);
	mode = 1;
	count = 0;
	while (argv[1][count])
	{
		if (argv[1][count] != ' ' || (token_word[0] == ':' && token_list.size() > 0))
			token_word += argv[1][count];
		else
		{
			mode = get_token(token_word, token_pair);
			token_list.push_back(token_pair);
			token_word.clear();
		}
		count++;
	}
	get_token(token_word, token_pair);
	token_list.push_back(token_pair);
	token_word.clear();

	std::vector<std::pair<std::string, std::string> >::iterator	iter_beg;
	std::vector<std::pair<std::string, std::string> >::iterator	iter_end;

	iter_beg = token_list.begin();
	iter_end = token_list.end();
	while (iter_beg != iter_end)
	{
		std::cout << iter_beg->first << ": " << iter_beg->second << std::endl;
		iter_beg++;
	}
	return (0);
}
