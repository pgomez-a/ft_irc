#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

static void	leaks(void)
{
	std::cout << "\n\n";
	system("leaks server");
	return ;
}

static void	manage_addrinfo(char* port, struct addrinfo& hints, struct addrinfo*& res)
{
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(0, port, &hints, &res) != 0)
	{
		std::cerr << "Error while calling getaddrinfo()\n";
		exit(1);
	}
	return ;
}

static int	create_socket(struct addrinfo*& res)
{
	int	sock_fd;

	sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock_fd == -1)
	{
		std::cerr << "Error while calling socket()\n";
		exit(1);
	}
	return (sock_fd);
}

static void	bind_socket(int sock_fd, struct addrinfo*& res)
{
	if (bind(sock_fd, res->ai_addr, res->ai_addrlen) == -1)
	{
		std::cerr << "Error while calling bind()\n";
		close(sock_fd);
		exit(1);
	}
	return ;
}

static void	listen_socket(int sock_fd, int capacity)
{
	if (listen(sock_fd, capacity) == -1)
	{
		std::cerr << "Error while calling listen()\n";
		close(sock_fd);
		exit(1);
	}
	return ;
}

static void	accept_socket(int sock_fd)
{
	int			nsock_fd;
	char			recv_buff[256];
	char			serv_addr[256];
	std::string		send_buff;
	socklen_t		client_len;
	struct sockaddr_storage	client_addr;

	client_len = sizeof(client_addr);
	nsock_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &client_len);
	if (nsock_fd == -1)
	{
		std::cerr << "Error while calling accept()\n";
		close(sock_fd);
		exit(1);
	}
	while (1)
	{
		send_buff = "Data received from ";
		std::memset(recv_buff, 0, 256);
		std::memset(serv_addr, 0, 256);
		if (recv(nsock_fd, recv_buff, 255, 0) <= 0)
		{
			std::cerr << "Error while calling recv()\n";
			close(nsock_fd);
			close(sock_fd);
			exit(1);
		}
		std::cout << "-> " << recv_buff;
		if (gethostname(serv_addr, 255) == -1)
		{
			std::cerr << "Error while calling gethostname()\n";
			close(nsock_fd);
			close(sock_fd);
			exit(1);
		}
		send_buff += static_cast<std::string>(serv_addr) + "\n";
		if (send(nsock_fd, send_buff.c_str(), send_buff.size(), 0) == -1)
		{
			std::cerr << "Error while calling send()\n";
			close(nsock_fd);
			close(sock_fd);
			exit(1);
		}
	}
	close(nsock_fd);
	return ;
}

int	main(int argc, char* argv[])
{
	int			sock_fd;
	struct addrinfo		hints;
	struct addrinfo*	res = 0;

	//atexit(leaks);
	if (argc != 2)
	{
		std::cerr << "Input error: ./server port\n";
		return (-1);
	}
	manage_addrinfo(argv[1], hints, res);
	sock_fd = create_socket(res);
	bind_socket(sock_fd, res);
	listen_socket(sock_fd, 10);
	accept_socket(sock_fd);
	freeaddrinfo(res);
	close(sock_fd);
	return (0);
}
