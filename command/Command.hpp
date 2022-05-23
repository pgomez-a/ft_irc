#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <list>
# include <iostream>

class Command
{
	public :
		Command(void);
		~Command(void);
	
		void		who_am_i(void);
		std::string	get_name(void);

	protected :

		std::string		_command_name;		
};

#endif