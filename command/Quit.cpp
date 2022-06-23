#include "Quit.hpp"

Quit::Quit(void) {
	_command_name = "QUIT";
	_id = QUIT;
};

int	Quit::_effect(server_t &server, client_t &client)
{
	std::string	quit_message;
	std::ofstream	history(".nameless_history", std::fstream::app);

	(void)server;
	quit_message = "Connection Closed for " + client.get_nick();
	if (_argc > 0)
		quit_message += ": " + _argt[0];
	else if (!_rest.empty())
		quit_message += ": " +  _rest;
	report_event(event_format(client.addr, client.port, quit_message), history, BLUE);
	history.close();
	return (0);
}
