#pragma once
#include <map>
#include <memory>
#include <vector>
#include <mutex>

class session;
class active_thread;
class event_loop
{
public:
	event_loop(int max_thread = 4);
	~event_loop();
	event_loop(const event_loop &loop) = delete;

	//event loop
	void loop();
	bool register_event(std::shared_ptr<session> ses, int event)
	bool unregister_event(std::shared_ptr<session> ses, int event);

private:
	bool update_event(int op, int fd, int event_opt);

private:
	std::map<int, std::shared_ptr<session>>  _handler;  //<<socket, clientsession>>
	int _event_fd;  //epoll_create
	int _wake_up_fd; //wake_up_fd for epoll_wait
	std::vector<std::shared_ptr<active_thread>>  _active_thread;
	std::mutex _lck_handler;
};

