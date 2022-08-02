#include "output.hpp"

/**
 ** Displays an error message through the error output.
 **/

void	put_error(std::string s)
{
	std::cerr << RED << "Error: " << s << RESET_COLOR << std::endl;
}

/**
 ** Indicates that an event has occurred through the standard output.
 **/

void	put_event(std::string message, std::string color)
{
	std::cout << color << message << RESET_COLOR << std::endl;
}

/**
 ** Saves an event that has occurred in the connection history.
 **/

void	log_event(std::string message, std::ofstream &history, std::string color, std::string dir)
{
	history << color << dir << " " << message << RESET_COLOR << std::endl;
}

/**
 ** Warns of an event and stores it in the connection history.
 **/

void	report_event(std::string message, std::ofstream &history, std::string color, std::string dir)
{
	put_event(message, color);
	log_event(message, history, color, dir);
}

/**
 ** Saves an event that has occurred in the connection history and returns the displayed message.
 **/

std::string	report_event_str(std::string message, std::ofstream &history, std::string color, std::string dir)
{
	std::string	event;

	event = color + message + RESET_COLOR + "\n";
	log_event(message, history, color, dir);
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
