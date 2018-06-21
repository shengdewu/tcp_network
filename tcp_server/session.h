#pragma once
#include <memory>

class tcp_server;
class event_loop;
class session
{
public:
	session(int fd, tcp_server *tcp_server, event_loop *event);
	virtual ~session();

	void notify_read_event();
	void notify_write_event();

	void send(std::string data, unsigned int size);

	inline int fd();
	
protected:
	//read/write operator
	virtual bool handle_read_event(char *data, unsigned int size) = 0;
	virtual bool handle_write_event() = 0;

private:
	//post read/write event to epoll
	void post_read_event();
	void post_write_event();

private:
	int	_fd;   //client fd
	tcp_serve *_tcp_server;
	event_loop *_event_loop;
};

///
//inline
//
inline int session::fd()
{
	return _fd;
}

