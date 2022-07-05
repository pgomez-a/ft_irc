#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <list>
# include <map>
# include "client.hpp"

#define MAX_CHANNEL_SIZE 32

enum	e_channel{BANNED, MEMBER_ADDED, MEMBER_HAS_BEEN_BANNED, MEMBER_NOT_IN_CHANNEL, MEMBER_DUPLICATE, CHANNEL_ADDED, CHANNEL_IS_FULL, TOO_MANY_CHANNELS, BAD_CHANNEL_NAME, BAD_TOPIC};

class Channel
{
	public :

		void			broadcast_message(const std::string &message) const; //enviar message con el formato correcto a todos los miembros
		int				add_member(client_t	*member);//cuando un cliente hace un join, puede devolver un msj de bienvenida, o error si el user ya está registrado
		int				ban_member(const std::string &nick);
		void			set_topic(const std::string &topic);
		std::string		get_topic(void) const;
		std::string		get_name(void) const;

		Channel(std::string name, std::string mode, std::string topic = "");

	private :

		std::string			_channel_message_format(std::string message) const;
		client_t			*_find_member(std::string nick);
		bool				_is_banned(std::string nick) const;
		bool				_delete_member(std::string nick);

		std::list<client_t *>	_member_list;
		size_t					_member_count;
		std::list<std::string>	_banned_list;
		std::string				_name;
		std::string				_mode;
		std::string				_topic;
};

#endif
