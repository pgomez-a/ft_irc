#ifndef REPLY_SYSTEM_HPP
# define REPLY_SYSTEM_HPP
# include "server.hpp"
# include "output.hpp"
# include "sys/socket.h"
# include "Command.hpp"

# define REPLY_NUMBER 500

enum	e_REPLIES{	
					RPL_WELCOME = 1,
					RPL_YOURHOST = 2,
					RPL_CREATED = 3,
					RPL_MYINFO = 4,
					RPL_NOTOPIC = 331,
					RPL_TOPIC = 332, 
					RPL_AWAY = 301, 
					RPL_YOUREOPER = 381,
				   	RPL_UMODEIS = 221,
					RPL_LIST = 322,
					RPL_LISTEND = 323,
					ERR_UNKNOWNCOMMAND = 421, 
					ERR_NONICKNAMEGIVEN = 431,
					ERR_NICKNAMEINUSE = 433,
					ERR_ERRONEUSNICKNAME = 432,
					ERR_NICKCOLLISION = 436,
					ERR_NEEDMOREPARAMS = 461, 
					ERR_ALREADYREGISTRED = 462,
					ERR_NOOPERHOST = 491,
					ERR_PASSWDMISMATCH = 464,
					ERR_INVITEONLYCHAN = 473, 
					ERR_CHANNELISFULL = 471, 
					ERR_NOSUCHCHANNEL = 403, 
					ERR_BANNEDFROMCHAN = 474, 
					ERR_BADCHANNELKEY = 475, 
					ERR_BADCHANMASK = 476,
					ERR_TOOMANYCHANNELS = 405,
					ERR_NOTONCHANNEL = 442,
					ERR_NORECIPIENT = 411, 
					ERR_CANNOTSENDTOCHAN = 404, 
					ERR_WILDTOPLEVEL = 414, 
					ERR_NOSUCHNICK = 401, 
					ERR_NOTEXTTOSEND = 412, 
					ERR_NOTOPLEVEL = 413,
					ERR_TOOMANYTARGETS = 407,
					ERR_NOORIGIN = 409,
					ERR_NOSUCHSERVER = 402,
					ERR_NOTREGISTERED = 451
				};

typedef			std::string (*reply)(client_t&, server_t&, Command *);
std::string		get_reply(int reply_code, client_t &client, Command *command);
std::string		reply_format(std::string command_code, std::string target, std::string arg, std::string message);
std::string		reply_format(std::string reply, std::string reply_code = "000");
int 			reply_to_client(int reply_code, client_t &client, server_t &server, Command *command);
int				send_to_client(std::string s, client_t &client);
int				welcome_new_registration(client_t &c, server_t &s, Command *n);
void			init_reply_matrix(reply*);

#endif
