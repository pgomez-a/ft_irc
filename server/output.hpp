#ifndef	OUTPUT_HPP
# define OUTPUT_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <sstream>

# define RESET_COLOR	"\033[0m"
# define BOLD			"\033[1m"
# define RED			"\033[91m"
# define YELLOW 		"\033[93m"
# define BLUE 			"\033[94m"

/** output.cpp **/
void		put_error(std::string s);

int			on_error(std::string s, int r);

void		put_event(std::string message, std::string color = RESET_COLOR);

void		log_event(std::string message, std::string color = RESET_COLOR, std::string dir = "<-");

void		report_event(std::string message, std::string color = RESET_COLOR, std::string dir = "<-");

std::string	report_event_str(std::string message, std::string color = RESET_COLOR, std::string dir = "<-");

std::string	event_format(std::string addr, std::string p, std::string msg);

#endif
