#include "output.hpp"

/**
 ** Displays an error message through the error output.
 **/

void	put_error(std::string s)
{
	std::cerr << RED << "Error: " << s << RESET_COLOR << std::endl;
	return ;
}

/**
 ** Displays an error message through the error output.
 **/

int		on_error(std::string s, int r)
{
	put_error(s);
	return r;
}

/**
 ** Indicates that an event has occurred through the standard output.
 **/

void	put_event(std::string message, std::string color)
{
	std::cout << color << message << RESET_COLOR << std::endl;
	return ;
}

/**
 ** Saves an event that has occurred in the connection history.
 **/

void	log_event(std::string message, std::string color, std::string dir)
{
	std::ofstream   history(".nameless_history", std::fstream::app);

	history << color << dir << " " << message << RESET_COLOR << std::endl;
	history.close();
	return ;
}

/**
 ** Warns of an event and stores it in the connection history.
 **/

void	report_event(std::string message, std::string color, std::string dir)
{
	put_event(message, color);
	log_event(message, color, dir);
	return ;
}

/**
 ** Saves an event that has occurred in the connection history and returns the displayed message.
 **/

std::string	report_event_str(std::string message, std::string color, std::string dir)
{
	std::string	event;

	event = color + message + RESET_COLOR + "\n";
	log_event(message, color, dir);
	return event;
}

/**
 ** Sets the address-port-message format.
 **/

std::string	event_format(std::string addr, std::string p, std::string msg)
{
	std::stringstream s;

	s << addr  << ":" << p << " " << msg;
	return s.str();
}
