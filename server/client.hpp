#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <netdb.h>

# define MAX_CLIENTS 1024
# define SERV_PASSW 1
# define CLI_NICK 2
# define CLI_USER 4
# define IS_REGISTERED (SERV_PASSW ^ CLI_NICK ^ CLI_USER)
# define STR(x) #x

struct server_t; //forward declaration

struct client_t
{
	public:

	/* 	Connection info */

		int					sock_fd;
		std::string			addr;
		std::string			port;
		struct sockaddr_in	info;
		
	/*member functions*/

		client_t(void);

		bool	operator==(client_t &rhs) const;
		bool	operator==(const client_t &rhs) const;
		
		void	register_flag(unsigned int flag);
		bool	flag_is_set(unsigned int flag) const;
		bool	registered(void) const;

		void		set_mode(std::string);
		std::string	get_mode(void) const;
		void		add_mode_flag(std::string flag);
		void		rm_mode_flag(std::string flag);
		bool		mode_flag_is_set(std::string flag);
		void		set_nick(std::string s);
		std::string	get_nick(void) const;
		void		set_user(std::string s);
		std::string	get_user(void) const;
		void		set_realname(std::string s);
		std::string	get_realname(void) const;
		std::string	get_originname(void) const;

		void		reset(void);

	private:

		int			_registration_flags;
		std::string	_mode;
		std::string	_nick;
		std::string	_user;
		std::string _realname;

};

/*
** Mode functions
*/

bool		valid_oper_credentials(std::string username, server_t &server, client_t &client);
std::string user_mode_bitmask(int m);

#endif
