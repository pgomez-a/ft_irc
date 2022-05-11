#include "ircserv.hpp"

/**
 ** accept    – accept a connection on a socket
 ** inet_ntoa - Internet address manipulation routines
 **
 ** int accept(int socket,                         char* inet_ntoa(struct int_addr in);
 **            struct sockaddr *restrict address,
 **            socklen_t *restrict address_len);
 **/

static int	accept_socket(int sock_fd, int poll_nfds, struct pollfd* poll_fds, struct sockaddr_in* hosts)
{
	int			nsock_fd;
	socklen_t		client_len;
	struct sockaddr_storage	client_addr;

	client_len = sizeof(client_addr);
	while (1)
	{
		std::memset(&client_addr, 0, client_len);
		nsock_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &client_len);
		if (nsock_fd == -1)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cerr << "\033[1m\033[91mError:\033[0m\033[91m accept()\n\033[0m";
				return (-1);
			}
			break ;
		}
		hosts[poll_nfds] = *((struct sockaddr_in*)&client_addr);
		poll_fds[poll_nfds].fd = nsock_fd;
		poll_fds[poll_nfds].events = POLLIN;
		std::cout << inet_ntoa(hosts[poll_nfds].sin_addr) << ":"
			<< hosts[poll_nfds].sin_port << " Connection Accepted\n";
		poll_nfds += 1;
	}
	return (poll_nfds);
}

/**
 ** recv – receive a message from a socket
 ** send – send a message from a socket
 **
 ** ssize_t recv(int socket,     ssize_t send(int socket,
 **              void *buffer,                const void *buffer,
 **              size_t length,               size_t length,
 **              int flags);                  int flags);
 **/

static int	read_socket(int sock_fd, int client_pos, struct sockaddr_in* hosts)
{
	int			recv_len;
	int			tmp_recv_len;
	char			recv_buff[212];
	std::string		send_buff;
	std::stringstream	port;

	recv_len = 0;
	tmp_recv_len = 0;
	std::memset(recv_buff, 0, 212);
	recv_buff[210] = '\n';
	recv_buff[211] = '\n';
	while (1)
	{
		recv_len = recv(sock_fd, recv_buff + tmp_recv_len, 210 - tmp_recv_len, 0);
		if (recv_len < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cerr << "\033[1m\033[91mError:\033[0m\033[91m recv()\n\033[0m";
				return (-1);
			}
			continue ;
		}
		tmp_recv_len += recv_len;
		if (recv_len == 0)
		{
			std::cout << inet_ntoa(hosts[client_pos].sin_addr) << ":"
				<< hosts[client_pos].sin_port << " Connection Closed\n";
			return (-1);
		}
		if (recv_buff[tmp_recv_len - 1] == '\n' || tmp_recv_len == 210)
		{
			std::cout << inet_ntoa(hosts[client_pos].sin_addr) << ":"
				<< hosts[client_pos].sin_port << " " << recv_buff;
			send_buff = (send_buff + inet_ntoa(hosts[0].sin_addr)) + ":";
			port << hosts[0].sin_port;
			send_buff += port.str() + " Received\n";
			if (send(sock_fd, send_buff.c_str(), send_buff.size(), 0) == -1)
			{
				std::cerr << "\033[1m\033[91mError:\033[0m\033[91m send()\n\033[0m";
				return (-1);
			}
			break ;
		}
	}
	return (0);
}

static int	reduce_poll_fds(int poll_nfds, struct pollfd* poll_fds)
{
	int	iter_y;
	int	iter_x;

	iter_y = 0;
	while (iter_y < poll_nfds)
	{
		if (poll_fds[iter_y].fd == -1)
		{
			iter_x = iter_y;
			while (iter_x < poll_nfds - 1)
			{
				poll_fds[iter_x].fd = poll_fds[iter_x + 1].fd;
				iter_x += 1;
			}
			poll_nfds -= 1;
			iter_y -= 1;
		}
		iter_y += 1;
	}
	return (poll_nfds);
}

/**
 ** close - close – delete a descriptor
 **
 ** int close(int fildes);
 **/

static void	close_poll_fds(int poll_nfds, struct pollfd* poll_fds)
{
	int	iter;

	iter = 0;
	while (iter < poll_nfds)
	{
		if (poll_fds[iter].fd >= 0)
			close(poll_fds[iter].fd);
		iter += 1;
	}
	return ;
}

/**
 ** poll - synchronous I/O multiplexing
 **
 ** int poll(struct pollfd fds[],
 **          nfds_t nfds,
 **          int timeout);
 **/

int		manage_socket(int sock_fd, struct sockaddr_in* server_info)
{
	int			reduced;
	int			end_server;
	int			func_return;
	int			poll_nfds;
	int			poll_tmp_nfds;
	int			poll_timeout;
	struct pollfd		poll_fds[200];
	struct sockaddr_in	hosts_info[200];

	reduced = 0;
	end_server = 0;
	std::memset(hosts_info, 0, sizeof(hosts_info));
	hosts_info[0] = *server_info;
	std::memset(poll_fds, 0, sizeof(poll_fds));
	poll_nfds = 1;
	poll_timeout = 5 * 60 * 1000;
	poll_fds[0].fd  = sock_fd;
	poll_fds[0].events = POLLIN;
	while(!end_server)
	{
		func_return = poll(poll_fds, poll_nfds, poll_timeout);
		if (func_return == -1)
		{
			std::cerr << "\033[1m\033[91mError:\033[0m\033[91m poll()\n\033[0m";
			break ;
		}
		if (func_return == 0)
		{
			std::cerr << "\033[1m\033[93mTimed out:\033[0m\033[93m poll()\n\033[0m";
			break ;
		}
		poll_tmp_nfds = poll_nfds;
		for (int i = 0; i < poll_tmp_nfds; i++)
		{
			if (poll_fds[i].revents == 0)
				continue ;
			if (!(poll_fds[i].revents & POLLIN))
			{
				std::cout << "\033[1m\033[91mError:\033[0m\033[91m revents\n\033[0m";
				end_server = 1;
				break ;
			}
			if (poll_fds[i].fd == sock_fd)
			{
				poll_nfds = accept_socket(sock_fd, poll_nfds, poll_fds, hosts_info);
				if (poll_nfds == -1)
				{
					end_server = 1;
					break ;
				}
			}
			else
			{
				func_return = read_socket(poll_fds[i].fd, i, hosts_info);
				if (func_return == -1)
				{
					close(poll_fds[i].fd);
					poll_fds[i].fd = -1;
					reduced = 1;
				}
			}
		}
		if (reduced)
		{
			reduced = 0;
			poll_nfds = reduce_poll_fds(poll_nfds, poll_fds);
		}
	}
	close_poll_fds(poll_nfds, poll_fds);
	std::cout << "\033[1m";
	std::cout << "Server disconnected...\n";
	std::cout << "For any problem contact the authors at https://github.com/pgomez-a/ft_irc\n";
	std::cout << "\033[0m\n";
	return (0);
}
