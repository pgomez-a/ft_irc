#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <fcntl.h>
# include <poll.h>
# include <list>
# include <map>
# include "client.hpp"
# include "channel.hpp"


//needs to be refactored, clients_info should be a map or similar instead of an array, and client_t should have
//comparison overload among other things
struct server_t
{
	public:

		typedef std::map<std::string, Channel>	channel_map;

		int					sock_fd;
		int					timeout;
		int					clients_nfds;
		//couldn't all these be private??
		std::string			name;
		std::string			addr;
		std::string			port;
		std::string			passwd;
		std::string			oper_passwd;
		struct addrinfo		hints;
		struct addrinfo*	res;
		struct pollfd		clients_fds[MAX_CLIENTS];
		client_t			clients_info[MAX_CLIENTS];

		client_t	*find_nick(std::string nick);
		client_t	*find_user(std::string user);

		bool		valid_oper_host(client_t &client);
		
		int						add_new_channel(std::string name, std::string mode, std::string topic);
		channel_map::iterator	channel_map_begin(void);
		channel_map::iterator	channel_map_end(void);
		size_t					channel_count(void);
		
	private:

		channel_map						_channel_map;
		std::list<std::string>			_no_oper_list;
};

//couldn't init server just be the constructor???
int	init_server(char* port, char* passwd, server_t& server);

/*
** Interaction
*/

bool client_is_in_server(const server_t &server, const client_t &client);
bool client_is_registered(const server_t &server, const client_t &client);


#endif
