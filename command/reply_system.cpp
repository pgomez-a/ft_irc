#include "reply_system.hpp"
#include "Command.hpp"

static std::string R_DUMMY(std::string c){ return "This is a dummy reply from " + c;}
static std::string R_ERR_UNKNOWNCOMMAND(std::string t, std::string c) {return reply_format("421", t, ":" + c + " :Unknown command");}
static std::string R_ERR_NEEDMOREPARAMS(std::string t, std::string c) {return reply_format("461", t, ":" + c + " :Not enough parameters");}
static std::string R_ERR_ALREADYREGISTERED(std::string t) {return reply_format("462", t, ":Unauthorize command (already registered)");}
static std::string R_ERR_NONICKNAMEGIVEN(std::string t) {return reply_format("431", t, " :No nickname given");}
static std::string R_ERR_NICKNAMEINUSE(std::string t) {return reply_format("433", t, ":" + c + " :Nickname is already in use");}


class Command;

std::string	get_reply(int reply_code, client_t &client, Command *command)
{
	std::string	reply_message;

	//change client.addr to client's nickname
	//should be table of commands associated with their corresponding codes
	switch (reply_code)
	{
	case ERR_UNKNOWNCOMMAND:
		reply_message = R_ERR_UNKNOWNCOMMAND(client.addr, command->get_name().c_str());
		break;
	case ERR_NEEDMOREPARAMS:
		reply_message = R_ERR_NEEDMOREPARAMS(client.addr, command->get_name().c_str());
		break;
	case ERR_ALREADYREGISTERED:
		reply_message = R_ERR_ALREADYREGISTERED(client.addr);
		break;
	default:
		reply_message = R_DUMMY(command->get_name());
		break;
	}
	return reply_message;
	(void)client, (void)command;
}

std::string		reply_format(std::string code, std::string target, std::string message)
{
	std::string	s;

	s += ":nameless " + code + " " + target + " :" + message + "\r\n";

	return s;
}

int reply_to_client(int reply_code, client_t &client, Command *command)
{
	std::string	reply_message;

	reply_message = get_reply(reply_code, client, command);
	return send(client.sock_fd, reply_message.c_str(), reply_message.size(), 0);
}