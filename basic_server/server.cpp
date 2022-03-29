#include <iostream>
#include <sys/socket.h> /** socket, bind, listen, accept, recv, send **/
#include <sys/types.h> /** ??? **/
#include <arpa/inet.h>  /** sockaddr_in **/
#include <unistd.h>     /** close **/

int	main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Error: ./server port\n";
		return (-1);
	}
	/**
	 ** 1. Crear socket
	 **
	 ** int socket(int domain, int type, int protocol)
	 **     domain: protocolo utilizado para la comunicación (IPV4 = AF_INET)
	 **     type: tipo de comunicación (TCP = SOCK_STREAM)
	 **     protocol: protocolo a usar, que por defecto es domain (por eso se pone a 0)
	 **
	 ** Por el momento solo disponemos de un fd correspondiente a un socket.
	 ** Todavía no está conectado a ningún puerto por el que poder conectarnos.
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
	 **
	 ** int bind(int socket, const struct sockaddr *address, socklen_t address_len)
	 **     socket: socket que queremos configurar
	 **     address: dirección de la estructura de configuración del socket
	 **     address_len: tamaño de la estructura de configuración del socket
	 **
	 ** Tras crear el socket, debemos configurarlo. Para ello creamos un struct sockaddr_in.
	 ** Con esta estrucrura determinamos el protocolo de comunicación (IPV4), la direcciones
	 ** a partir de las cuales permitir las comunicaciones, el puerto al que conectarse.
	 **
	 **     sockaddr_in.sin_family: protocolo de comunicación (IPV4 = AF_INET)
	 **     sockaddr_in.sin_addr.s_addr: direcciones que pueden conectarse al socket
	 ***    sockaddr_in.sin_port: puerto al que está conectado el socket
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
	 **
	 ** int listen(int socket, int backlog)
	 **     socket: socket que ya hemos configurado y necesitamos activar
	 **     backlog: número máximos de conexiones pendientes
	 **
	 ** Ahora que hemos configurado el socket, necesitamos permitir dicha comunicación.
	 ** Por defecto, pese a que el socket ya está conectado al puerto en cuestión,
	 ** debemos permitir activamente que pueda recibir información. Esta activación consciente
	 ** la hacemos a través de la función listen. A partir de este momento cualquier IP autorizada
	 ** tras el bind puede enviar información al socket.
	 **/

	if (listen(sock_fd, 10) == -1)
	{
		std::cerr <<  "Error: Can't activate socket to listen\n";
		std::cerr << "errno = " << errno << std::endl;
		return (-1);
	}

	/**
	 ** 4. Aceptar conexión entrante
	 **
	 ** int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len)
	 **     socket: socket que recibe la conexión
	 **     address: estructura para configurar el nuevo socket
	 **     address_len: longitud de la estructura para configurar el socket
	 **
	 ** Con accept se toma la primera conexión de la pila de conexiones pendientes, se crea
	 ** un nuevo socket con las características del mismo socket al que se envía la petición,
	 ** y devuelve un fd para referirnos a dicho socket.
	 ** Si el socket no está marcado como no-bloqueante, accept espera hasta recibir una conexión.
	 ** En caso contrario, accept devuelve un error.
	 ** El nuevo socket no debería usarlo para gestionar más conexiones, pues el socket principal
	 ** va a seguir abierto.
	 **/

	socklen_t	hint_len;
	int		new_sock_fd;
	int		tot;

	tot = 3;
	hint_len = sizeof(hint);
	while (tot > 0)
	{
		new_sock_fd = accept(sock_fd, (struct sockaddr*)&hint, (socklen_t*)&hint_len);
		if (new_sock_fd == -1)
		{
			std::cerr << "Error: Can't accept incoming connection\n";
			std::cerr << "errno = " << errno << std::endl;
			return (-1);
		}

		char*	ip = inet_ntoa(hint.sin_addr);

		std::cout << "New client #" << tot << " from " << ip << "\n";
		
		/**
		 ** 5. Gestionar recibo de información
		 **
		 ** ssize_t recv(int socket, void *buffer, size_t length, int flags)
		 **         socket: nuevo socket devuelto por accept
		 **         buffer: buffer donde guardar la info recivida
		 **         lenght: máxima longitud de caracteres que se pueden leer
		 **
		 ** Una vez que el socket recibe información, debe ser capaz de gestionarla.
		 ** Con recv almacenamos la información enviada al socket en un buffer al mismo tiempo que
		 ** nos devuelve la cantidad de bytes que han sido enviados.
		 **
		 ** ssize_t send(int socket, const void *buffer, size_t length, int flags)
		 **         socket: nuevo socket devuelto por accept
		 **         buffer: info que se envia al cliente como respuesta
		 **         lenght: máxima longitud de caracteres que se pueden enviar
		 **
		 ** Debemos enviar un mensaje de vuelta a la dirección IP que nos ha enviado
		 ** la información recibida, para que sepa que se ha recibido su petición.
		 **/

		char		buff[101];
		int		len_buff;
		std::string	response;

		len_buff = recv(new_sock_fd, buff, 100, 0);
		if (len_buff == -1)
		{
			std::cerr << "Error: Can't manage received info\n";
			std::cerr << "errno = " << errno << std::endl;
			return (-1);
		}
		buff[len_buff] = '\0';
		std::cout << "Message: " << buff;

		response = "Data received...\n";
		if (send(new_sock_fd, response.c_str(), response.size(), 0) == -1)
		{
			std::cerr << "Error: Can't sent request to client\n";
			std::cerr << "errno = " << errno << std::endl;
			return (-1);
		}
		close(new_sock_fd);
		tot--;
	}
	close(sock_fd);
	std::cout << "\033[1m\033[32mSERVER HAS BEEN CLOSED\033[39m\033[1m\n";
	return (0);
}
