#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <list>

# define MAX_CLIENTS 1024
# define SERV_PASSW 1
# define CLI_NICK 2
# define CLI_USER 4
# define IS_REGISTERED (SERV_PASSW ^ CLI_NICK ^ CLI_USER)
# define STR(x) #x

/** Forward Declarations **/
struct server_t;
class Channel;

enum e_reset_mode{SOFT_RESET, HARD_RESET};

struct	joined_channel
{
	joined_channel(Channel *channel, std::string mode);

	Channel 	*c;
	std::string	m;
};

struct client_t
{
	public:
		/** Public Member Attributes **/
		int					sock_fd;
		std::string			addr;
		std::string			port;
		struct sockaddr_in	info;
		
		/** Constructor & Destructor **/
		client_t(void);
		~client_t(void);

		/** Operator Overloads **/
		bool	operator==(client_t &rhs) const;
		bool	operator==(const client_t &rhs) const;
		
		/** Member Methods **/
		void	register_flag(unsigned int flag);
		bool	flag_is_set(unsigned int flag) const;
		bool	registered(void) const;
		int		add_mode_flag(std::string flag);
		int		rm_mode_flag(std::string flag);
		bool	mode_flag_is_set(std::string flag);
		void	reset(int reset_mode = SOFT_RESET);
		void	add_channel_to_list(Channel *c, std::string m = "empty");
		bool	pop_channel_from_list(Channel *c);
		void	clear_channel_list(void);
		bool	is_in_channel(Channel &c);

		/** Getters & Setters **/
		void		set_mode(std::string);
		std::string	get_mode(void) const;
		void		set_nick(std::string s);
		std::string	get_nick(void) const;
		void		set_user(std::string s);
		std::string	get_user(void) const;
		void		set_realname(std::string s);
		std::string	get_realname(void) const;
		std::string	get_originname(void) const;

	private:
		/** Private Member Attributes **/
		int							_registration_flags;
		std::list<joined_channel>	_channel_list;
		std::string					_mode;
		std::string					_nick;
		std::string					_user;
		std::string 				_realname;
};

/** Mode functions **/
bool		valid_oper_credentials(std::string username, server_t &server, client_t &client);
std::string user_mode_bitmask(int m);

/** Interaction **/
bool	client_is_in_server(const server_t &server, const client_t &client);
bool	client_is_registered(const server_t &server, const client_t &client);

#endif
