#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <typeinfo>

# include "reply_system.hpp"
# include "Command.hpp"
# include "Nick.hpp"
# include "User.hpp"
# include "Error.hpp"
# include "Pass.hpp"
# include "Oper.hpp"
# include "Quit.hpp"
# include "Join.hpp"
# include "Privmsg.hpp"
# include "Notice.hpp"
# include "Part.hpp"
# include "Ping.hpp"
# include "Cap.hpp"
# include "List.hpp"
# include "Topic.hpp"
# include "Mode.hpp"
# include "Names.hpp"

enum	e_COMMANDS{NICK, USER, PASS, CAP, ERROR, OPER, QUIT, JOIN, PART, PRIVMSG, NOTICE, PING, PONG, LIST, TOPIC, MODE, NAMES};

void	add_flag(std::string &modes, char flag);
void	rm_flag(std::string &modes, char flag);
bool	flag_is_set(std::string modes, char flag);
bool	is_operator(std::string modes);

#endif
