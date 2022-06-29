#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <list>
# include <map>
# include "client.hpp"

class Channel
{
	public :

		void			broadcast_message(const std::string &message) const; //enviar message con el formato correcto a todos los miembros
		int				add_member(client_t	*member);//cuando un cliente hace un join, puede devolver un msj de bienvenida, o error si el user ya está registrado
		std::string		ban_member(const std::string &nick);

	private :

		std::string			_channel_message_format(std::string message) const;
		client_t			*_find_member(std::string nick);
		std::string			_delete_member(std::string nick);

		std::list<client_t *>	_member_list;
		std::list<std::string>	_banned_list;
		std::string				_mode;
		std::string				_topic;

};

#endif
