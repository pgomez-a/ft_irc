#include "server.hpp"
#include "word_composition.hpp"

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

server_t::server_t(void):
sock_fd(), timeout(), clients_nfds(), name("nameless"), addr("nameless.org"), port(), passwd(), oper_passwd("oper"), hints(), res(), clients_fds(), clients_info(), _channel_map(), _no_oper_list()
{}

int	init_server(char* port, char* passwd, server_t& server)
{
	server.port = static_cast<std::string>(port);
	server.passwd = static_cast<std::string>(passwd);
	server.oper_passwd = "oper"; //harcoded access to Operator flag //harcoded access to Operator flag
	//server.version = "0.1";
	server.timeout = 5 * 1000 * 60;
	server.clients_fds[0].fd = server.sock_fd;
	server.clients_fds[0].events = POLLIN;
	server.clients_nfds = 1;
	return (0);
}

/**
 ** Channel interaction
 **/

int	server_t::add_new_channel(std::string name, std::string mode, std::string topic, channel_map::iterator &i)
{
	if (valid_channelname(name))
	{
		if (valid_rest(topic))
		{
			i = _channel_map.insert(std::make_pair(name, Channel(name, mode, topic))).first;
			return CHANNEL_ADDED; 
		}
		return BAD_TOPIC;
	}
	return BAD_CHANNEL_NAME;
}

server_t::channel_map::iterator	server_t::find_channel(std::string name)
{
	return _channel_map.find(name);
}

server_t::channel_map::iterator	server_t::channel_map_begin(void) 
{
	return _channel_map.begin();
}

server_t::channel_map::iterator	server_t::channel_map_end(void) 
{
	return _channel_map.end();
}

size_t	server_t::channel_count(void)
{
	return _channel_map.size();
}
