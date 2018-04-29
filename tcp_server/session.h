#pragma once
class session
{
public:
	session();
	virtual ~session();

	//读写事件处理
	void notify_read_event();
	void notify_write_event();

protected:
	//读写数据
	virtual bool handle_read_event() = 0;
	virtual bool handle_write_event() = 0;

private:
	//注册读写事件
	void post_read_event();
	void post_write_event();

private:
	int	_fd;   //套接字
};

