#include "output.hpp"

void	put_error(std::string s)
{
	std::cerr << RED << "Error: " << s << RESET_COLOR << std::endl;
}

void	put_event(std::string message, std::string color)
{
	std::cout << color << message << RESET_COLOR << std::endl;
}

void	log_event(std::string message, std::ofstream &history, std::string color, std::string dir)
{
	history << color << dir << " " << message << RESET_COLOR << std::endl;
}

void	report_event(std::string message, std::ofstream &history, std::string color, std::string dir)
{
	put_event(message, color);
	log_event(message, history, color, dir);
}

std::string	report_event_str(std::string message, std::ofstream &history, std::string color, std::string dir)
{
	std::string	event = color + message + RESET_COLOR + "\n";
	log_event(message, history, color, dir);

	return event;
}

std::string	event_format(std::string addr, std::string p, std::string msg)
{
	std::stringstream s;

	s << addr  << ":" << p << " " << msg;

	return s.str();
}