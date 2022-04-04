#include <iostream>
#include <sys/socket.h> /** socket, bind, listen, accept, recv, send **/
#include <sys/types.h>  /** ??? **/
#include <arpa/inet.h>  /** sockaddr_in **/
#include <unistd.h>     /** close **/
#include <netdb.h>      /** gethostbyname **/

struct	t_server
{
	public:
		struct hostent*	host_info;       // Struct with all the info related to the host
		char		host_name[256];  // Name of the host
		char*		host_addr;  // Ip of the host
};

int	main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Error: ./server port\n";
		return (-1);
	}
	/**
	 ** 1. Crear socket
	 **/

	int	sock_fd;

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1)
	{
		std::cerr << "Error: Can't create the socket\n";
		std::cerr << "errno = " << errno << std::endl;
		return (-1);
	}

	/**
	 ** 2. Configurar socket
	 **/

	sockaddr_in	hint;
	int		port;

	port = std::stoi(argv[1]);
	hint.sin_family = AF_INET;
	hint.sin_addr.s_addr = INADDR_ANY; // Se permite la conexión desde cualquier IP
	hint.sin_port = htons(port);
	if (bind(sock_fd, (sockaddr*)&hint, sizeof(hint)) == -1)
	{
		std::cerr << "Error: Can't bind the socket\n";
		std::cerr << "errno = " << errno << std::endl;
		return (-1);
	}

	/**
	 ** 3. Poner en modo escucha
	 **/

	if (listen(sock_fd, 10) == -1)
	{
		std::cerr <<  "Error: Can't activate socket to listen\n";
		std::cerr << "errno = " << errno << std::endl;
		return (-1);
	}

	/**
	 ** 4. Aceptar conexión entrante
	 **/

	socklen_t	hint_len;
	int		new_sock_fd;

	hint_len = sizeof(hint);
	new_sock_fd = accept(sock_fd, (struct sockaddr*)&hint, (socklen_t*)&hint_len);
	if (new_sock_fd == -1)
	{
		std::cerr << "Error: Can't accept incoming connection\n";
		std::cerr << "errno = " << errno << std::endl;
		return (-1);
	}

	t_server	server_info;

	gethostname(server_info.host_name, sizeof(server_info.host_name));
	server_info.host_info = gethostbyname(server_info.host_name);
	server_info.host_addr = inet_ntoa(*((struct in_addr*)server_info.host_info->h_addr_list[0]));

	while(1)
	{
		/**
		 ** 5. Gestionar recibo de información
		 **/

		char		buff[511];
		int		len_buff;
		std::string	response;

		len_buff = recv(new_sock_fd, buff, 511, 0);
		if (len_buff == -1)
		{
			std::cerr << "Error: Can't manage received info\n";
			std::cerr << "errno = " << errno << std::endl;
			return (-1);
		}
		if (len_buff <= 511)
			buff[len_buff] = '\0';
		else
			buff[510] = '\0';
		std::cout << buff;

		response = static_cast<std::string>(server_info.host_addr);
		response += " :Welcome to the Internet Relay Network, <nick>!<user>@";
		response += static_cast<std::string>(inet_ntoa(hint.sin_addr)) + "\n";
		if (send(new_sock_fd, response.c_str(), response.size(), 0) == -1)
		{
			std::cerr << "Error: Can't sent request to client\n";
			std::cerr << "errno = " << errno << std::endl;
			return (-1);
		}
	}
	close(new_sock_fd);
	close(sock_fd);
	std::cout << "\033[1m\033[32mSERVER HAS BEEN CLOSED\033[39m\033[1m\n";
	return (0);
}
