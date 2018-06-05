#pragma once
#include <memory>

class session
{
public:
	session(int fd);
	virtual ~session();

	void notify_read_event();
	void notify_write_event();

	void send(std::shared_ptr<char*> data, unsigned int size);

	inline int fd(){return _fd;}
	
protected:
	//read/write operator
	virtual bool handle_read_event(char *data, unsigned int size) = 0;
	virtual bool handle_write_event() = 0;

private:
	//post read/write event to epoll
	void post_read_event();
	void post_write_event();

private:
	int	_fd;   //�׽���
};

