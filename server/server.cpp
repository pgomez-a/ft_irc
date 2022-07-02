#include "server.hpp"
#include ""

client_t	*server_t::find_nick(std::string nick)
{
	for (int i = 0; i < clients_nfds; ++i)
	{
		if (clients_info[i].get_nick() == nick)
			return &clients_info[i];
	}
	return NULL;
}

client_t	*server_t::find_user(std::string user)
{
	for (int i = 0; i < clients_nfds; ++i)
	{
		if (clients_info[i].get_user() == user)
			return &clients_info[i];
	}
	return NULL;
}

bool	server_t::valid_oper_host(client_t &client)
{
	for (std::list<std::string>::iterator i = _no_oper_list.begin(); i != _no_oper_list.end(); ++i)
	{
		if (client.addr == *i)
			return false;
	}
	return true;
}

/**
 ** Inits server values before manage incoming connections.
 **/

int	init_server(char* port, char* passwd, server_t& server)
{
	server.name = "nameless";
	server.addr = "nameless";
	server.port = static_cast<std::string>(port);
	server.passwd = static_cast<std::string>(passwd);
	server.oper_passwd = "oper"; //harcoded access to Operator flag //harcoded access to Operator flag
	//server.version = "0.1";
	server.timeout = 5 * 1000 * 60;
	std::memset(server.clients_info, 0, sizeof(server.clients_info));
	std::memset(server.clients_fds, 0, sizeof(server.clients_fds));
	server.clients_fds[0].fd = server.sock_fd;
	server.clients_fds[0].events = POLLIN;
	server.clients_nfds = 1;
	return (0);
}

/**
 ** Channel interaction
 **/

int	Channel::add_new_channel(std::string name, std::string mode, std::string topic)
{
	Channel	new_channel(name, mode, topic);

	if (valid_channelname(name))
	{
		if (valid_rest(topic))
		{
			channel_map.insert(std::pair(name, new_channel));
			return CHANNEL_ADDED; 
		}
		return BAD_TOPIC;
	}
	return BAD_CHANNEL_NAME;
}

channel_map::iterator	channel_map_begin(void)
{
	return _channel_map.begin();
}

channel_map::iterator	channel_map_end(void)
{
	return _channel_map.begin();
}
