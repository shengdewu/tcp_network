#include <functional>
#include <sys/epoll.h>
#include "tcp_server.h"
#include "acceptor.h"
#include "event_loop.h"
#include "session.h"


tcp_server::tcp_server()
:_acceptor(std::make_shared<acceptor>(std::bind(&tcp_server::handle_new_connected, this, std::placeholders::_1))),
 _event_loop(std::make_shared<event_loop>())
{
}


tcp_server::~tcp_server()
{
}

bool tcp_server::init_tcp(std::string ip, unsigned int port)
{
	return _acceptor->start(ip, port);
}

void tcp_server::handle_new_connected(int fd)
{
	auto session = _connected.find(fd);
	if (session != _connected.end())
	{
		return;
	}

	_connected[fd]  = create_session(fd);

	_event_loop->register_event(_connected[fd], EPOLLIN|EPOLLOUT|EPOLLET);
}

