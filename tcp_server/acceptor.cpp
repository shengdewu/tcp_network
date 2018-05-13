#include "acceptor.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(port);
	//bind
	if(!bind(_listen_fd, (struct sockaddr*)(&addr),sizeof(addr)))
	{
		std::cout << "bind [" << ip <<":"<<port<<"] failed" << std::endl;
		return false;
	}
	//listen
	if(!listen(_listen_fd, 5))
	{
		std::cout << "listen" << ip <<":"<<port<<"] failed" << std::endl;
		return false;
	}
	_exit = false;
	_listen_th = std::thread(std::bind(&acceptor::loop, this));
}

void acceptor::stop()
{
	_exit = true;
	shutdown(_listen_fd, SHUT_WR);
	close(_listen_fd);
	_listen_th.join();
}

void acceptor::loop()
{
	while (!_exit)
	{
		//accept
		struct sockaddr_in client_addr;
		int len = sizeof(client_addr);
		int client_fd = accept(_listen_fd, (struct sockaddr*)(&client_addr),&len);
		if(client_fd < 0) 
		{
			continue;
		}

		char client_ip[INET_ADDRSTRLEN] ={'\0'};
		inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(cleint_ip));
		std::cout << "accept [" << client_ip <<":" << ntohs(client_addr.sin_port) << "] conected " << std::endl;
		
		notify_new_connect(client_fd);
		
	}
}

void acceptor::notify_new_connect(int cfd)
{
	if (_notify_con)
	{
		_notify_con(cfd);
	}
}
