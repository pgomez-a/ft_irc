#include "ircserv.hpp"

/**
 ** Accepts all incoming client connections that are in the backlog.
 **  - inet_ntoa: converts ip_address to char*
 **/

static int	find_new_client_pos(int nfds, server_t &server)
{
	int	pos;

	pos = nfds;
	while (pos > 0)
	{
		if (server.clients_fds[pos].fd == -1)
			break ;
		--pos;
	}
	return pos == 0 ? nfds : pos;
}

static int	register_new_client(int nfds, int nsock_fd, struct sockaddr_storage client_addr, server_t &server)
{
	int			pos;
	std::string	event;

	pos = find_new_client_pos(nfds, server);
	server.clients_info[pos].info = *((struct sockaddr_in*)&client_addr);
	server.clients_info[pos].sock_fd = nsock_fd;
	server.clients_info[pos].addr = inet_ntoa(server.clients_info[pos].info.sin_addr);
	server.clients_info[pos].port = std::to_string(server.clients_info[pos].info.sin_port);
	server.clients_fds[pos].fd = nsock_fd;
	server.clients_fds[pos].events = POLLIN;
	event = event_format(server.clients_info[pos].addr, server.clients_info[pos].port,"Connection Accepted");
	report_event(event, YELLOW);
	if (pos == nfds)
		nfds += 1;
	return nfds;
}

static int	accept_socket(server_t &server)
{
	int						nfds;
	int						nsock_fd;
	socklen_t				client_len;
	struct sockaddr_storage	client_addr;

	nfds = server.clients_nfds;
	client_len = sizeof(client_addr);
	while (1)
	{
		std::memset(&client_addr, 0, client_len);
		nsock_fd = accept(server.sock_fd, (struct sockaddr*)&client_addr, &client_len);
		if (nsock_fd == -1 && errno == EWOULDBLOCK)
			break ;
		else if (nsock_fd == -1)
			return on_error("accept()", -1);
		nfds = register_new_client(nfds, nsock_fd, client_addr, server);
	}
	return nfds;
}

/**
 ** Handles a client request and responds with the appropriate reply
 **/

static int	read_socket(client_t &client, server_t &server)
{
	int				recv_len;
	int				tmp_recv_len;
	char			recv_buff[212];
	std::string		quit_message;

	tmp_recv_len = 0;
	std::memset(recv_buff, 0, 212);
	recv_buff[210] = '\r';
	recv_buff[211] = '\n';
	while (1)
	{
		recv_len = recv(client.sock_fd, recv_buff + tmp_recv_len, 210 - tmp_recv_len, 0);
		if (recv_len < 0 && errno != EWOULDBLOCK)
			return on_error("recv()", -2);
		else if (recv_len < 0)
			continue ;
		if (recv_len == 0)
		{
			quit_message = client.get_nick().empty() ? "Connection Closed": "Connection Closed: " + client.get_nick() + " disconnected";
			report_event(event_format(client.addr, client.port, quit_message), BLUE);
			return -1;
		}
		tmp_recv_len += recv_len;
		if ((recv_buff[tmp_recv_len - 2] == '\r' && recv_buff[tmp_recv_len - 1] == '\n' )|| tmp_recv_len == 210)
			return execute_client_request(recv_buff, tmp_recv_len, server, client);
	}
	return 0;
}

/**
 ** Closes all the client sockets that are still opened just after the server has decided to stop.
 **/

static void	close_poll_fds(server_t &server)
{
	for (int iter = 0; iter < server.clients_nfds; ++iter)
	{
		if (server.clients_fds[iter].fd >= 0)
			close(server.clients_fds[iter].fd);
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

static int	manage_request(server_t &server)
{
	int	end_server;
	int	func_return;
	int	poll_tmp_nfds;

	end_server = 0;
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
			server.clients_nfds = accept_socket(server);
		else
		{
			func_return = read_socket(server.clients_info[i], server);
			if (func_return < 0)
			{
				if (func_return == -2)
					end_server = 1;
				close(server.clients_fds[i].fd);
				server.clients_fds[i].fd = -1;
				server.clients_info[i].reset(HARD_RESET);
			}
		}
	}
	return end_server;
}

int			manage_socket(server_t &server)
{
	int	end_server;
	int	func_return;

	end_server = 0;
	while(!end_server)
	{
		func_return = poll(server.clients_fds, server.clients_nfds, server.timeout);
		if (func_return == -1)
		{
			put_error("poll()");
			break ;
		}
		if (func_return == 0)
		{
			put_error("timeout()");
			break ;
		}
		end_server = manage_request(server);
	}
	close_poll_fds(server);
	put_event("Server disconnected...\nContact the authors at https://github.com/pgomez-a/ft_irc\n");
	return 0;
}
