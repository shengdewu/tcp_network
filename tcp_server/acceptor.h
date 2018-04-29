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

	//����
	bool start(std::string ip, unsigned int port);
	
	void stop();

protected:
	//�����¼�ѭ���߳�
	void loop();
	//�����ӣ�֪ͨ�ӿ�
	void notify_new_connect(int cfd);

private:
	int			 _listen_fd;
	std::thread  _listen_th;
	new_connect	 _notify_con;
	bool		 _exit;
};

