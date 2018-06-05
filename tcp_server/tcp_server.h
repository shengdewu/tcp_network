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
	//����������
	bool init_tcp(std::string ip, unsigned int port);

protected:
	//����������
	void handle_new_connected(int fd);
	//�����Ự
	virtual std::shared_ptr<session> create_session(int fd);

private:
	std::map<int, std::shared_ptr<session>> _connected; //<�׽��֣��Ự>
	std::shared_ptr<acceptor>	_acceptor;
	
};

