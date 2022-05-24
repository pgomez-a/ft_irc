#include "Command.hpp"

class Error : public Command
{
	public : 

		Error(void);

		using Command::parentheses;

		void	parentheses(std::string origin, size_t argc, std::string *argt, std::string rest);

	private :

};