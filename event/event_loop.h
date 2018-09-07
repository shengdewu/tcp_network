#pragma once
#include <map>
#include <memory>
#include <vector>
#include "public/rwlock.h"

class active_thread;
class event_handler;
class event_loop
{
public:
	event_loop(int max_thread = 4);
	~event_loop();
	event_loop(const event_loop &loop) = delete;

	//event loop
	void loop();
	//注册事件
	bool register_event(int fd, std::shared_ptr<event_handler> handler);
	//删除事件
	bool delete_handler(int fd);
	//更新事件
	bool update_event(int op, int fd, int events, bool once = true);

private:
	void init_delete_epoll();
	void delete_epoll();
	std::shared_ptr<event_handler> find_handler(int fd);

private:
	std::map<int, std::shared_ptr<event_handler>>  _handler;  //<<socket, event_handler>>
	int _event_fd;  //epoll_create
	int _wake_up_fd; //wake_up_fd for epoll_wait
	std::vector<std::shared_ptr<active_thread>>  _active_thread;
	rwlock _lck_handler;
};

