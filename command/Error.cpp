#include "Error.hpp"

Error::Error(void) {
	_command_name = "ERROR";
};

void	Error::parentheses(std::string origin, size_t argc, std::string *argt, std::string rest)
{
	std::cout << "I am an overloaded error member\n";
	(void)origin, (void)argc, (void)argt, (void)rest;
}
