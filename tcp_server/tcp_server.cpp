#include "tcp_server.h"


tcp_server::tcp_server()
{
}


tcp_server::~tcp_server()
{
}

bool tcp_server::init_tcp(std::string ip, unsigned int port)
{

}

void tcp_server::handle_new_connected(int fd)
{
	auto session = _connected.find(fd);
	if (session != _connected.end())
	{
		return;
	}

	_connected[fd] = create_session(fd);
}

std::shared_ptr<session> tcp_server::create_session(int fd)
{
	return std::make_shared<session>(fd);
}
