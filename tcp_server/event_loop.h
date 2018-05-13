#pragma once
#include <map>
#include <memory>

class session;
class event_loop
{
public:
	event_loop();
	~event_loop();
	event_loop(const event_loop &loop) = delete;

	//事件循环
	void loop();
	//注册事件
	bool register_event();

private:
	std::map<int, std::shared_ptr<session>>  _handler;  //<<套接字，事件句柄>>
};

