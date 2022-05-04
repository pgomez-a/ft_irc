	#include <map>
	#include "parser.hpp"
	#include "commands.hpp"

//	static	Command	_command_array[] = {User(), Nick()};

	static std::map<std::string, Command *>	command_map;

	static void	init_command_map(void)
	{
		static	Command command_array[] = {Nick(), User()};

		command_map.insert(std::make_pair("NICK", &command_array[NICK]));
		command_map.insert(std::make_pair("USER", &command_array[USER]));
	}

	command_list message_parser(token_list tokens)
	{
		command_list	commands;

		if (command_map.empty())
			init_command_map();
		for (token_list::iterator	i = tokens.begin(); i != tokens.end(); ++i)
		{
			//temporary test, need a proper testing routine
			if (i->type == WORD && command_map.count(i->content))
			{
				std::cout << "VALID CMD FOUND, it says:\n";
				(command_map.find(i->content))->second->who_am_i();
			}
		}
		return commands;
	}