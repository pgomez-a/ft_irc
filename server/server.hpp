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

struct server_t
{
	public:
		/** Member Type **/
		typedef std::map<std::string, Channel>	channel_map;

		/** Public Member Attributes **/
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

		/** Constructor & Destructor **/
		server_t(void);
		~server_t(void);

		/** Member Methods **/
		client_t	*find_nick(std::string nick);
		client_t	*find_user(std::string user);
		bool		valid_oper_host(client_t &client);
		int			add_new_channel(std::string name, std::string mode, std::string topic, channel_map::iterator &i);
		void		del_channel(channel_map::iterator i);

		/** Channel Methods **/
		channel_map::iterator	find_channel(std::string name);
		channel_map::iterator	channel_map_begin(void);
		channel_map::iterator	channel_map_end(void);
		size_t					channel_count(void);
		
	private:
		/** Private Member Attributes **/
		channel_map				_channel_map;
		std::list<std::string>  _no_oper_list;
};

#endif
