#include "reply_system.hpp"
#include "Command.hpp"

class Command;

static void compiler_treat(client_t &c, server_t &s, Command *n) 
{(void)c, (void)s, (void)n;}

static std::string R_DUMMY(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return "";}

static std::string R_WELCOME(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("001", c.addr, "", (std::string)"Welcome to the Internet Relay Network " + c.get_nick() + "!" + c.get_user() + "@" + c.addr);}

static std::string R_YOURHOST(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("002", c.addr, "", "Your host is " + s.name + ", running version 0.000001");}

static std::string R_CREATED(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("003", c.addr, "", (std::string)"This server was created  " + "<date>");}

static std::string R_MYINFO(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("004", c.addr, "" , s.name + " " + "0.01" + "<available user modes>" + "\n" + "<available channel modes>");}

static std::string R_NOTOPIC(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("331", c.addr, "<channel>", "No topic is set");}

static std::string R_TOPIC(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("332", c.addr, "<channel>", "<topic>");}

static std::string R_AWAY(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("301", c.addr, c.get_nick(), "<away message>");}

static std::string R_YOUREOPER(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("381", c.addr, "", "You are now an IRC operator");}

static std::string R_ERR_UNKNOWNCOMMAND(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("421", c.addr, n->get_name(), "Unknown command");}

static std::string R_ERR_NEEDMOREPARAMS(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("461", c.addr,  n->get_name(), "Not enough parameters");}

static std::string R_ERR_ALREADYREGISTRED(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("462", c.addr, "", "Unauthorized command (already registered)");}

static std::string R_ERR_NONICKNAMEGIVEN(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("431", c.addr, "", "No nickname given");}

static std::string R_ERR_NICKNAMEINUSE(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("433", c.addr, (*n)[0], "Nickname is already in use");}

static std::string R_ERR_ERRONEUSNICKNAME(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("432", c.addr, "", "Erroneous nickname");}

static std::string R_ERR_NICKCOLLISION(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("436", c.addr, (*n)[0], (std::string)"Nickname collision KILL from " + "user" + "@" + "host");} //replace with proper info

static std::string R_ERR_NOOPERHOST(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("491", c.addr, "", "No O-lines for your host");}

static std::string R_ERR_PASSWDMISMATCH(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("464", c.addr, "", "Password incorrect");}

static std::string R_ERR_INVITEONLYCHAN(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("473", c.addr, (std::string)"<channel>", "Cannot join channel (+i)");}

static std::string R_ERR_CHANNELISFULL(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("471", c.addr, (std::string)"<channel>", "Cannot join channel (+l)");}

static std::string R_ERR_NOSUCHCHANNEL(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("403", c.addr, (std::string)"<channel>", "No such channel");}

static std::string R_ERR_BANNEDFROMCHAN(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("474", c.addr, (std::string)"<channel>", "Cannot join channel (+b)");}

static std::string R_ERR_BADCHANNELKEY(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("475", c.addr,(std::string)"<channel>", "Cannot join channel (+k)");}

static std::string R_ERR_BADCHANMASK(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("476", c.addr, (std::string)"<channel>", "Bad Channel Mask");}

static std::string R_ERR_TOOMANYCHANNELS(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("405", c.addr, (std::string)"<channel>", "You have joined too many channels");}

static std::string R_ERR_NOTONCHANNEL(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("442", c.addr, (std::string)"<channel>", "You're not on that channel");}

static std::string R_ERR_NORECIPIENT(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("411", c.addr,"", "No recipient given (" + n->get_name() + ")");}

static std::string R_ERR_CANNOTSENDTOCHAN(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("404", c.addr, (std::string)"<channel>", "Cannot send to channel");}

static std::string R_ERR_WILDTOPLEVEL(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("414", c.addr, (std::string)"<mask>", "Wilcard in top level");}

static std::string R_ERR_NOSUCHNICK(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("401", c.addr, (*n)[0], "No such nick/channel");}

static std::string R_ERR_NOTEXTTOSEND(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("412", c.addr,"", "No text to send");}

static std::string R_ERR_NOTOPLEVEL(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("413", c.addr, (std::string)"<mask>", "No toplevel domain specified");}

static std::string R_ERR_TOOMANYTARGETS(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("407", c.addr, "", "407 recipients. <abort message>");}

static std::string R_ERR_NOSUCHSERVER(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("402", c.addr, c.addr, "No such server");}

static std::string R_ERR_NOORIGIN(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("409", c.addr, "", "No origin specified");}

static std::string R_ERR_NOTREGISTRED(client_t &c, server_t &s, Command *n)
{compiler_treat(c,s,n); return reply_format("451", c.addr, "", "You have not registered.");}

void	init_reply_matrix(reply *reply_matrix)
{
	reply_matrix[0] = R_DUMMY;
	reply_matrix[RPL_WELCOME] = R_WELCOME;
	reply_matrix[RPL_YOURHOST] = R_YOURHOST;
	reply_matrix[RPL_CREATED] = R_CREATED;
	reply_matrix[RPL_MYINFO] = R_MYINFO;
	reply_matrix[RPL_TOPIC] = R_TOPIC;
	reply_matrix[RPL_NOTOPIC] = R_NOTOPIC;
	reply_matrix[RPL_AWAY] = R_AWAY;
	reply_matrix[RPL_YOUREOPER] = R_YOUREOPER;
	reply_matrix[ERR_UNKNOWNCOMMAND] = R_ERR_UNKNOWNCOMMAND;
	reply_matrix[ERR_NONICKNAMEGIVEN] = R_ERR_NONICKNAMEGIVEN;
	reply_matrix[ERR_NICKNAMEINUSE] = R_ERR_NICKNAMEINUSE;
	reply_matrix[ERR_ERRONEUSNICKNAME] = R_ERR_ERRONEUSNICKNAME;
	reply_matrix[ERR_NICKCOLLISION] = R_ERR_NICKCOLLISION;
	reply_matrix[ERR_NEEDMOREPARAMS] = R_ERR_NEEDMOREPARAMS;
	reply_matrix[ERR_ALREADYREGISTRED] = R_ERR_ALREADYREGISTRED;
	reply_matrix[ERR_NOOPERHOST] = R_ERR_NOOPERHOST;
	reply_matrix[ERR_NOORIGIN] = R_ERR_NOORIGIN;
	reply_matrix[ERR_PASSWDMISMATCH] = R_ERR_PASSWDMISMATCH;
	reply_matrix[ERR_INVITEONLYCHAN] = R_ERR_INVITEONLYCHAN;
	reply_matrix[ERR_CHANNELISFULL] = R_ERR_CHANNELISFULL;
	reply_matrix[ERR_NOSUCHCHANNEL] = R_ERR_NOSUCHCHANNEL;
	reply_matrix[ERR_BANNEDFROMCHAN] = R_ERR_BANNEDFROMCHAN;
	reply_matrix[ERR_BADCHANNELKEY] = R_ERR_BADCHANNELKEY;
	reply_matrix[ERR_BADCHANMASK] = R_ERR_BADCHANMASK;
	reply_matrix[ERR_TOOMANYCHANNELS] = R_ERR_TOOMANYCHANNELS;
	reply_matrix[ERR_NOTONCHANNEL] = R_ERR_NOTONCHANNEL;
	reply_matrix[ERR_NORECIPIENT] = R_ERR_NORECIPIENT;
	reply_matrix[ERR_CANNOTSENDTOCHAN] = R_ERR_CANNOTSENDTOCHAN;
	reply_matrix[ERR_WILDTOPLEVEL] = R_ERR_WILDTOPLEVEL;
	reply_matrix[ERR_NOSUCHNICK] = R_ERR_NOSUCHNICK;
	reply_matrix[ERR_NOSUCHSERVER] = R_ERR_NOSUCHSERVER;
	reply_matrix[ERR_NOTEXTTOSEND] = R_ERR_NOTEXTTOSEND;
	reply_matrix[ERR_NOTOPLEVEL] = R_ERR_NOTOPLEVEL;
	reply_matrix[ERR_TOOMANYTARGETS] = R_ERR_TOOMANYTARGETS;
	reply_matrix[ERR_NOTREGISTERED] = R_ERR_NOTREGISTRED;
	}

std::string	get_reply(int reply_code, client_t &client, server_t &server, Command *command)
{
	static reply	reply_matrix[REPLY_NUMBER] = {};
	std::string		reply_message;

	reply_code = (reply_code > REPLY_NUMBER) ? 0 : reply_code;
	if (reply_matrix[0] == NULL)
		init_reply_matrix(reply_matrix);
	reply_message = reply_matrix[reply_code](client, server, command);
	return reply_message;
}

std::string		reply_format(std::string code, std::string target, std::string arg, std::string message)
{
	std::string	s;

	s += ":nameless " + code + " " + target +  " " + arg + " :" + message + "\r\n";

	return s;
}

int reply_to_client(int reply_code, client_t &client, server_t &server,Command *command)
{
	std::string	reply_message;

	reply_message = get_reply(reply_code, client, server, command);
	return send_to_client(reply_message, client);
}

int send_to_client(std::string &s, client_t &client)
{
	ssize_t		r;

	r = 0;
	if (s.size())
		r = send(client.sock_fd, s.c_str(), s.size(), 0);
	return r;
}

int	welcome_new_registration(client_t &c, server_t &s, Command *n)
{
	std::string welcome = R_WELCOME(c, s, n);
	std::string your_host = R_YOURHOST(c, s, n);
	std::string created = R_CREATED(c, s, n);

	send_to_client(welcome, c);
	send_to_client(your_host, c);
	send_to_client(created, c);
	return 0;
}
