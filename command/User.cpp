#include "User.hpp"

User::User(void) {
	_command_name = "USER";
	_id = USER;
};

int	User::_effect(server_t &server, client_t &client)
{

	if (_argc >= 4 || (_argc == 3 && !_rest.empty()))
	{
		if (client.flag_is_set(CLI_USER))
			return ERR_ALREADYREGISTRED;
		if (valid_user(_argt[0]))
		{
			client.set_user(_argt[0]);
			client.set_mode(user_mode_bitmask(atoi(_argt[1].c_str())));
			client.set_realname((_rest.empty()) ? _argt[4]: _rest);
			client.register_flag(CLI_USER);
			send_to_client(reply_format(":nameless MODE " + client.get_nick() + " :" + client.get_mode()), client);
			if (client.registered())
				return welcome_new_registration(client, server, this);
		}
		return 0;
	}
	return ERR_NEEDMOREPARAMS;
}
