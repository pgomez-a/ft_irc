#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <list>
# include <map>
# include "client.hpp"

#define MAX_CHANNEL_SIZE 32

enum	e_channel{BANNED, MEMBER_ADDED, MEMBER_HAS_BEEN_BANNED, MEMBER_NOT_IN_CHANNEL, MEMBER_DUPLICATE,
		CHANNEL_ADDED, CHANNEL_IS_FULL, TOO_MANY_CHANNELS, BAD_CHANNEL_NAME, BAD_TOPIC};

struct	ban_t
{
	ban_t(std::string b, std::string o);

	std::string ban_mask;
	std::string	origin;
};

class Channel
{
	public :
		/** Constructor & Destructor **/
		Channel(std::string name, std::string mode, std::string topic = "");
		~Channel(void);

		/** Member Methods **/
		void	broadcast_message(client_t &sender, std::string command, const std::string &message) const;
		void	broadcast_message(client_t &sender, std::string command, const std::string &message, bool send_to_self) const;
		int		add_member(client_t	*member);
		bool	delete_member(std::string nick);
		int		ban(const std::string originname, std::string ban_origin);
		void	unban(const std::string ban_mask);
		bool	is_member_on_channel(std::string nick);
		bool	is_banned(client_t *client) const;
		bool	is_banned(std::string oname) const;
		int		add_mode_flag(std::string flag);
		int		rm_mode_flag(std::string flag);
		bool	mode_flag_is_set(std::string flag);

		/** Getters & Setters **/
		void					set_topic(const std::string &topic);
		std::string				get_topic(void) const;
		std::string				get_name(void) const;
		size_t					get_member_count(void) const;
		std::list<client_t *>	get_member_list(void);
		std::string				get_member_list(char separator);
		std::string				get_mode(void);
		std::list<client_t *>	get_member_list_by_origin_name(std::string originname);
		std::list<ban_t>		get_banned_list(void);

	private :
		/** Private Member Attributes **/
		std::list<client_t *>	_member_list;
		size_t					_member_count;
		std::list<ban_t>		_banned_list;
		std::string				_name;
		std::string				_mode;
		std::string				_topic;

		/** Private Methods **/
		client_t	*_find_member(std::string nick);
};

#endif
