#include "acceptor.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno>
#include <string.h> //for strerror()
#include <fcntl.h>

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
	bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
	addr.sin_port = htons(port);
	//bind
	if(-1 == bind(_listen_fd, &addr, sizeof(addr)))
	{
		std::cout << "bind socket is failed " << strerror(errno) << std::endl;
		close(_listen_fd);
	}
	//listen

	_exit = false;
	_listen_th = std::thread(std::bind(&acceptor::loop, this));
}

void acceptor::stop()
{
	close(_listen_fd);
	_exit = true;
	_listen_th.join();	
}

void acceptor::loop()
{
	while (!_exit)
	{
		struct sockaddr_in addr;
		bzero(&addr, sizeof(struct sockaddr_in));
		int len = sizeof(addr);
		
		int client = accept(_listen_fd, (sockaddr*)&addr, &len);
		if(client < 0)
		{
			std::cout << "accept socket is failed " << strerror(errno) << std::endl;
		}

		char ip[20] = {'\0'};
		inet_ntop(AF_INET, &addr, ip, 20);
		std::cout << "ip:"<< ip << "-port:"<< ntohs(addr.sin_port) << " connect the server."<< std::endl;
		notify_new_connect(client);
	}
}

void acceptor::notify_new_connect(int cfd)
{
	if (_notify_con)
	{
		int old=fcntl(fd,F_GETFL);
		int newfd=old|O_NONBLOCK;
		fcntl(client,F_SETFL,newfd);

		_notify_con(cfd);
	}
}
