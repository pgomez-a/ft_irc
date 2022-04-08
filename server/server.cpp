#include "server.hpp"

/**
static void	leaks(void)
{
	std::cout << "\n\n";
	system("leaks server");
	return ;
}
**/

static void	manage_addrinfo(char* port, struct addrinfo& hints, struct addrinfo*& res)
{
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_DEFAULT;
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

static void	accept_socket(int sock_fd, struct addrinfo*& res)
{
	int			nsock_fd;
	int			recv_len;
	char			recv_buff[212];
	std::string		send_buff;
	socklen_t		client_len;
	struct sockaddr_storage	client_addr;
	struct sockaddr_in*	client_info;
	struct sockaddr_in*	server_info;

	client_len = sizeof(client_addr);
	nsock_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &client_len);
	if (nsock_fd == -1)
	{
		std::cerr << "Error while calling accept()\n";
		close(sock_fd);
		exit(1);
	}
	server_info = (struct sockaddr_in*)res->ai_addr;
	client_info = (struct sockaddr_in*)&client_addr;
	std::cout << inet_ntoa(client_info->sin_addr) << ":" << client_info->sin_port << " Connection accepted\n";
	std::memset(recv_buff, 0, 212);
	recv_len = 0;
	while (1)
	{
		recv_len += recv(nsock_fd, recv_buff + recv_len, 210 - recv_len, 0);
		if (recv_len <= 0)
		{
			if (recv_len == 0)
				break ;
			std::cerr << "Error while calling recv()\n";
			close(nsock_fd);
			close(sock_fd);
			exit(1);
		}
		else if (recv_buff[recv_len - 1] == '\n' || recv_len == 210)
		{
			recv_buff[210] = '\n';
			std::cout << inet_ntoa(client_info->sin_addr) << ":" << client_info->sin_port << " " << recv_buff;
			send_buff = "Data received from ";
			send_buff = (send_buff + inet_ntoa(server_info->sin_addr)) + "\n";
			if (send(nsock_fd, send_buff.c_str(), send_buff.size(), 0) == -1)
			{
				std::cerr << "Error while calling send()\n";
				close(nsock_fd);
				close(sock_fd);
				exit(1);
			}
			std::memset(recv_buff, 0, 212);
			recv_len = 0;
		}
	}
	close(nsock_fd);
	std::cout << inet_ntoa(client_info->sin_addr) << ":" << client_info->sin_port << " Connection ended\n";
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
	accept_socket(sock_fd, res);
	freeaddrinfo(res);
	close(sock_fd);
	return (0);
}
