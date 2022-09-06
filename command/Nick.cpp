#include "Nick.hpp"

Nick::Nick(void) {
	_command_name = "NICK";
	_id = NICK;
};

int	Nick::_effect(server_t &server, client_t &client)
{	
	std::string old_originname = client.get_originname();
	std::list<joined_channel> channel_list;
	bool		p;

	if (!_argc)
		return ERR_NONICKNAMEGIVEN;
	if  (valid_nickname(_argt[0]))
	{
		if (server.find_nick(_argt[0]))
			return ERR_NICKNAMEINUSE;
		p = !client.registered();
		client.register_flag(CLI_NICK);
		client.set_nick(_argt[0]);
		if (!p)
		{
			send_to_client(":" + old_originname +" NICK :" + _argt[0] + "\r\n", client);			
			channel_list = client.get_joined_channel_list();
			for (std::list<joined_channel>::iterator i = channel_list.begin(); i != channel_list.end(); ++i)
			{
				i->chan->broadcast_message(client, old_originname + " NICK"  + " :" + client.get_nick(), 0);
			}	
		}
		return (p && client.registered()) ? welcome_new_registration(client, server, this) : 0;
	}
	return ERR_ERRONEUSNICKNAME;
}
