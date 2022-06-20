#include "ircserv.hpp"


static	int	on_error(std::ofstream &history, std::string error, int r)
{
	put_error(error);
	history.close();
	return r;
}

/**
 ** Accepts all incoming client connections that are in the backlog.
 **  - inet_ntoa: converts ip_address to char*
 **/

static int	accept_socket(server_t& server)
{
	int						nfds;
	int						nsock_fd;
	socklen_t				client_len;
	struct sockaddr_storage	client_addr;
	std::ofstream			history(".nameless_history", std::fstream::app);
	std::string				event;

	nfds = server.clients_nfds;
	client_len = sizeof(client_addr);
	while (1)
	{
		std::memset(&client_addr, 0, client_len);
		nsock_fd = accept(server.sock_fd, (struct sockaddr*)&client_addr, &client_len);
		if (nsock_fd == -1)
		{
			if (errno != EWOULDBLOCK)
				return on_error(history, "accept()", -1);
			break ;
		}
		server.clients_info[nfds].info = *((struct sockaddr_in*)&client_addr);
		server.clients_info[nfds].sock_fd = nsock_fd;
		server.clients_info[nfds].addr = inet_ntoa(server.clients_info[nfds].info.sin_addr);
		server.clients_info[nfds].port = std::to_string(server.clients_info[nfds].info.sin_port);
		server.clients_fds[nfds].fd = nsock_fd;
		server.clients_fds[nfds].events = POLLIN;
		event = event_format(server.clients_info[nfds].addr, server.clients_info[nfds].port,"Connection Accepted");
		report_event(event, history, YELLOW);
		nfds += 1;
	}
	history.close();
	return (nfds);
}

/**
 ** Handles a client request and responds with the appropriate reply
 **/

static int	read_socket(client_t &client, server_t server)
{
	int				recv_len;
	int				tmp_recv_len;
	int				split_executor;
	char			recv_buff[212];
	std::string		send_buff;
	std::ofstream	history(".nameless_history", std::fstream::app);

	recv_len = 0;
	tmp_recv_len = 0;
	std::memset(recv_buff, 0, 212);
	recv_buff[210] = '\r';
	recv_buff[211] = '\n';
	while (1)
	{
		recv_len = recv(client.sock_fd, recv_buff + tmp_recv_len, 210 - tmp_recv_len, 0);
		if (recv_len < 0)
		{
			if (errno != EWOULDBLOCK)
				return on_error(history, "recv()", -2);
			continue ;
		}
		if (recv_len == 0)
		{
			report_event(event_format(client.addr, client.port, "Connection Closed"), history, BLUE);
			history.close();
			return (-1);
		}
		tmp_recv_len += recv_len;
		if ((recv_buff[tmp_recv_len - 2] == '\r' && recv_buff[tmp_recv_len - 1] == '\n' )|| tmp_recv_len == 210)
		{
			recv_len = 0;
			split_executor = 0;
			while (recv_buff[recv_len])
			{
				if (recv_len > 0 && recv_buff[recv_len] == '\n' && recv_buff[recv_len - 1] == '\r')
				{
					recv_buff[recv_len - 1] = '\0';
					report_event(event_format(client.addr, client.port, recv_buff + split_executor), history);
					if (executor(recv_buff + split_executor, tmp_recv_len - 1, server, client) == -1)
				 		return on_error(history, "send()", -2);
					split_executor = recv_len + 1;
				}
				recv_len += 1;
			}
			break ;
		}
	}
	history.close();
	return (0);
}

/**
 ** Adjusts clients_fds struct erasing all the clients that have been disconnected.
 **/

static int	reduce_poll_fds(server_t& server)
{
	int	iter_y;
	int	iter_x;

	iter_y = 0;
	while (iter_y < server.clients_nfds)
	{
		if (server.clients_fds[iter_y].fd == -1)
		{
			iter_x = iter_y;
			while (iter_x < server.clients_nfds - 1)
			{
				server.clients_fds[iter_x].fd = server.clients_fds[iter_x + 1].fd;
				server.clients_info[iter_x] = server.clients_info[iter_x  + 1];
				iter_x += 1;
			}
			server.clients_nfds -= 1;
			iter_y -= 1;
		}
		iter_y += 1;
	}
	return (server.clients_nfds);
}

/**
 ** Closes all the client sockets that are still opened just after the server has decided to stop.
 **/

static void	close_poll_fds(server_t& server)
{
	int	iter;

	iter = 0;
	while (iter < server.clients_nfds)
	{
		if (server.clients_fds[iter].fd >= 0)
			close(server.clients_fds[iter].fd);
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

int		manage_socket(server_t& server)
{
	int	reduced;
	int	end_server;
	int	func_return;
	int	poll_tmp_nfds;

	reduced = 0;
	end_server = 0;
	while(!end_server)
	{
		func_return = poll(server.clients_fds, server.clients_nfds, server.timeout);
		if (func_return == -1)
		{
			put_error("poll()");
			break ;
		}
		if (func_return == 0) // extra condition??
		{
			put_error("poll()");
			break ;
		}
		poll_tmp_nfds = server.clients_nfds;
		for (int i = 0; i < poll_tmp_nfds; i++)
		{
			if (server.clients_fds[i].revents == 0)
				continue ;
			if (!(server.clients_fds[i].revents & POLLIN))
			{
				put_error("revents");
				end_server = 1;
				break ;
			}
			if (server.clients_fds[i].fd == server.sock_fd)
			{
				server.clients_nfds = accept_socket(server);
				if (server.clients_nfds == -1)
				{
					end_server = 1;
					break ;
				}
			}
			else
			{
				func_return = read_socket(server.clients_info[i], server);
				if (func_return < 0)
				{
					if (func_return == -2)
						end_server = 1;
					close(server.clients_fds[i].fd);
					server.clients_fds[i].fd = -1;
					server.clients_info[i].reset();
					reduced = 1;
				}
			}
		}
		if (reduced)
		{
			reduced = 0;
			server.clients_nfds = reduce_poll_fds(server);
		}
	}
	close_poll_fds(server);
	put_event("Server disconnected...\nContact the authors at https://github.com/pgomez-a/ft_irc\n");
	return (0);
}
