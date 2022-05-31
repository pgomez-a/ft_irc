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

/*
PASS: used to set a 'connection password'. The password must be set before any attempt to register 
the connection is made. The password supplied must match the server password. It is possible to send multiple PASS 
commands before registering but only the last one sent is used for verification and it may not be changed once registered.
Command: PASS <password>
Numeric Replies: ERR_NEEDMOREPARAMS   ERR_ALREADYREGISTRED

3.1.2 Nick message


      Command: NICK
   Parameters: <nickname>

   NICK command is used to give user a nickname or change the existing
   one.

Numeric Replies:

           ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
           ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
           ERR_UNAVAILRESOURCE             ERR_RESTRICTED

   Examples:

   NICK Wiz                ; Introducing new nick "Wiz" if session is
                           still unregistered, or user changing his
                           nickname to "Wiz"

   :WiZ!jto@tolsun.oulu.fi NICK Kilroy
                           ; Server telling that WiZ changed his
                           nickname to Kilroy.

3.1.3 User message

      Command: USER
   Parameters: <user> <mode> <unused> <realname>

   The USER command is used at the beginning of connection to specify
   the username, hostname and realname of a new user.

   The <mode> parameter should be a numeric, and can be used to
   automatically set user modes when registering with the server.  This
   parameter is a bitmask, with only 2 bits having any signification: if
   the bit 2 is set, the user mode 'w' will be set and if the bit 3 is
   set, the user mode 'i' will be set.  (See Section 3.1.5 "User
   Modes").

   The <realname> may contain space characters.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

   Example:

   USER guest 0 * :Ronnie Reagan   ; User registering themselves with a
                                   username of "guest" and real name
                                   "Ronnie Reagan".

   USER guest 8 * :Ronnie Reagan   ; User registering themselves with a
                                   username of "guest" and real name
                                   "Ronnie Reagan", and asking to be set
                                   invisible.



OPER: used by a normal user to obtain operator privileges. 
The combination of <user> and <password> are required to gain Operator privileges.
Command: OPER <user> <password>
Numeric Replies: ERR_NEEDMOREPARAMS   ERR_NOOPERHOST   RPL_YOUREOPER   ERR_PASSWDMISMATCH


QUIT: a client session if ended with a quit message. 
The server must close the connection to a client which sends a QUIT message. 
If a "Quit Message" is given, this will be sent instead of the default message, the nickname. 
If, for some other reason, a client connection is closed without the client 
issuing a QUIT command (e.g. client dies and EOF occurs on socket), the server is required to fill in 
the quit message with some sort of message reflecting the nature of the event which caused it to happen.
Command: QUIT [<Quit message>]
Numeric Replies: None

JOIN: used by client to start listening a specific channel. The conditions which affect this are as follows:
1 - the user must be invited if the channel is invite-only;
2 - the user's nick/username/hostname must not match any active bans;
3 - the correct key (password) must be given if it is set.
Once a user has joined a channel, they receive notice about all
commands their server receives which affect the channel. 
This includes PART, QUIT and of course PRIVMSG/NOTICE. 
If a JOIN is successful, the user is then sent the channel's topic 
(using RPL_TOPIC) and the list of users who are on the channel (using RPL_NAMREPLY), 
which must include the user joining.
Command: JOIN <channel>{,<channel>} [<key>{,<key>}]
Numeric Replies: ERR_NEEDMOREPARAMS   ERR_INVITEONLYCHAN   ERR_CHANNELISFULL   ERR_NOSUCHCHANNEL   RPL_TOPIC   ERR_BANNEDFROMCHAN   ERR_BADCHANNELKEY   ERR_BADCHANMASK   ERR_TOOMANYCHANNELS


PART: causes the client sending the message to be removed from the list of 
active users for all given channels listed in the parameter string.
Command: PART <channel>{,<channel>}
Numeric Replies: ERR_NEEDMOREPARAMS   ERR_NOTONCHANNEL &emps; ERR_NOSUCHCHANNEL

PRIVMSG: used to send private messages between users. <receiver> is the nickname 
of the receiver of the message. <receiver> can also be a list of names or channels separated with commas.
Command: PRIVMSG <receiver>{,<receiver>} <text to be sent>
Numeric Replies: ERR_NORECIPIENT   ERR_CANNOTSENDTOCHAN   ERR_WILDTOPLEVEL   ERR_NOSUCHNICK   RPL_AWAY   ERR_NOTEXTTOSEND   ERR_NOTOPLEVEL   ERR_TOOMANYTARGETS

NOTICE: The NOTICE message is used similarly to PRIVMSG. 
The difference between NOTICE and PRIVMSG is that automatic 
replies must never be sent in response to a NOTICE message.
Command: NOTICE <nickname> <text>
*/

enum	e_COMMANDS{NICK, USER, ERROR, PASS, OPER, QUIT, JOIN, PART, PRIVMSG, NOTICE};
#endif