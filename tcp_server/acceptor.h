#pragma once
#include <thread>
#include <functional>

class acceptor
{
public:
	typedef std::function<void(int)> new_connect;

public:
	acceptor(new_connect notify = nullptr);
	virtual ~acceptor();

	//监听
	bool start(std::string ip, unsigned int port);
	
	void stop();

protected:
	//监听事件循环线程
	void loop();
	//新连接，通知接口
	void notify_new_connect(int cfd);

private:
	int			 _listen_fd;
	std::thread  _listen_th;
	new_connect	 _notify_con;
	bool		 _exit;
};

