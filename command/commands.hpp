#ifndef COMMANDS_HPP
# define COMMANDS_HPP

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

/*
PASS: used to set a 'connection password'. The password must be set before any attempt to register 
the connection is made. The password supplied must match the server password. It is possible to send multiple PASS 
commands before registering but only the last one sent is used for verification and it may not be changed once registered.
Command: PASS <password>
Numeric Replies: ERR_NEEDMOREPARAMS   ERR_ALREADYREGISTRED

NICK: used to give user a nickname or change the previous one. If a NICK message arrives at a server 
which already knows about an identical nickname for another client, a nickname collision occurs. 
As a result of a nickname collision, all instances of the nickname are removed from the server's database, 
and a KILL command is issued to remove the nickname from all other server's database. If the NICK message causing the collision 
was a nickname change, then the original (old) nick must be removed as well.
If the server recieves an identical NICK from a client which is directly connected, it may issue an 
ERR_NICKCOLLISION to the local client, drop the NICK command, and not generate any kills.
Command: NICK <nickname>
Numeric Replies: ERR_NONICKNAMEGIVEN   ERR_NICKNAMEINUSE   ERR_ERRONEUSNICKNAME   ERR_NICKCOLLISION


USER: used at the beginning of connection to specify the username,
hostname, servername and realname of a new server. Only after both USER and NICK have been received from a 
client does a user become registered. It must be noted that realname parameter must be the last parameter, because it may 
contain space characters and must be prefixed with a colon (':') to make sure this is recognised as such.
Command: USER <username> <hostname> <servername> <realname>
Numeric Replies: ERR_NEEDMOREPARAMS   ERR_ALREADYREGISTRED


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