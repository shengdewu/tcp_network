#include "tcp_server.h"
#include "acceptor.h"


tcp_server::tcp_server()
{
}


tcp_server::~tcp_server()
{
	if(_acceptor)
	{
		_acceptor.stop();
	}
}

bool tcp_server::init_tcp(std::string ip, unsigned int port)
{
	if(!_acceptor)
	{
		return true;
	}

	_acceptor = make_shared<acceptor>(std::bind(&tcp_server::handle_new_connected, this, std::placeholders::_1));
	_acceptor->start(ip, port);
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
