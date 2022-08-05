#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <list>
# include <map>
# include "client.hpp"

#define MAX_CHANNEL_SIZE 32

enum	e_channel{BANNED, MEMBER_ADDED, MEMBER_HAS_BEEN_BANNED, MEMBER_NOT_IN_CHANNEL, MEMBER_DUPLICATE,
		CHANNEL_ADDED, CHANNEL_IS_FULL, TOO_MANY_CHANNELS, BAD_CHANNEL_NAME, BAD_TOPIC};

class Channel
{
	public :
		/** Constructor & Destructor **/
		Channel(std::string name, std::string mode, std::string topic = "");
		~Channel(void);

		/** Member Methods **/
		void	broadcast_message(client_t &sender, std::string command, const std::string &message) const;
		int		add_member(client_t	*member);
		bool	delete_member(std::string nick);
		int		ban_member(const std::string &nick);
		bool	is_member_on_channel(std::string nick);

		/** Getters & Setters **/
		void		set_topic(const std::string &topic);
		std::string	get_topic(void) const;
		std::string	get_name(void) const;
		size_t		get_member_count(void) const;
		std::string	get_member_list(char separator);
		std::string	get_mode(void);

	private :
		/** Private Member Attributes **/
		std::list<client_t *>	_member_list;
		size_t					_member_count;
		std::list<std::string>	_banned_list;
		std::string				_name;
		std::string				_mode;
		std::string				_topic;

		/** Private Methods **/
		client_t	*_find_member(std::string nick);
		bool		_is_banned(std::string nick) const;
};

#endif
