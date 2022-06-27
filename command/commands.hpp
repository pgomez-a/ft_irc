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

enum	e_COMMANDS{NICK, USER, PASS, CAP, ERROR, OPER, QUIT, JOIN, PART, PRIVMSG, NOTICE, PING, PONG};

#endif
