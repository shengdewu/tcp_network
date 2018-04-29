#include "acceptor.h"
#include <sys/socket.h>
#include <netinet/in.h>

acceptor::acceptor(new_connect notify)
:_notify_con(notify),
_exit(false)
{
}


acceptor::~acceptor()
{
	stop();
}

bool acceptor::start(std::string ip, unsigned int port)
{
	//create socket
	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	//bind

	//listen

	_exit = false;
	_listen_th = std::thread(std::bind(&acceptor::loop, this));
}

void acceptor::stop()
{
	_exit = true;
	_listen_th.join();
}

void acceptor::loop()
{
	while (!_exit)
	{
		//accept
	}
}

void acceptor::notify_new_connect(int cfd)
{
	if (_notify_con)
	{
		_notify_con(cfd);
	}
}
