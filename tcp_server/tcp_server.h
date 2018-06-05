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
	
	bool init_tcp(std::string ip, unsigned int port);

protected:
	//create new session for new connect
	virtual std::shared_ptr<session> create_session(int fd) = 0;

private:
	//listen call back for new connect
	void handle_new_connected(int fd);	

private:
	std::map<int, std::shared_ptr<session>> _connected; //<�׽��֣��Ự>
	std::shared_ptr<acceptor> _acceptor;
	std::shared_ptr<event_loop> _event_loop;
};

