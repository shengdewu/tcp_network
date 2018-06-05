#pragma once
#include <map>
#include <memory>

class event_loop;
class session;
class acceptor;
class tcp_server
{
public:
	tcp_server();
	~tcp_server();
	//启动服务器
	bool init_tcp(std::string ip, unsigned int port);

protected:
	//处理新连接
	void handle_new_connected(int fd);
	//创建会话
	virtual std::shared_ptr<session> create_session(int fd);

private:
	std::map<int, std::shared_ptr<session>> _connected; //<套接字，会话>
	std::shared_ptr<acceptor>	_acceptor;
	
};

