#include "event_loop.h"
#include <sys/socket.h>
#include "active_thread.h"
#include <sys/epoll.h>
#include <errno.h>
#include "session.h"

#define MAX_EVENTS_SIZE 32

event_loop::event_loop(int max_thread)
	:_event_fd(epoll_create(EPOLL_CLOEXEC)),
	_active_thread(std::make_shared<active_thread>(max_thread)),
	_exit((false)
{
	_active_thread->post(std::bind(&event_loop::loop, this));
}


event_loop::~event_loop()
{
	_exit = true;
	//wake up wait
}

void event_loop::loop()
{
	while(!_exit)
	{
		struct epoll_event events[MAX_EVENTS_SIZE] = {0};
		int nfd = epoll_wait(_event_fd, &events, sizeof(evenkts), 10000);
		if(nfd > 0)
		{
			for(int i=0; i<nfd; ++i)
			{
				auto it = _handler.find(evenkts[i].data.fd);
				if(it == _handler.end())
				{
					continue;
				}

				switch (evenkts[i].events)
				{
					case EPOLLIN:
						it.second->notify_read_event();
					break;
					case EPOLLOUT:
						it.second->notify_write_event();
					break;
					default:
					break;
				}
			}
		}
		else if(nfd == 0)
		{
			std::cout << "event_loop::loop have nothing." << errno << std::endl;
		}
		else
		{
			std::cout << "event_loop::loop" << errno << std::endl;
		}
	}
}

bool event_loop::register_event(std::shared_ptr<session> ses, int event)
{
	std::lock_guard<std::mutex> guard(lck_handler_);
	auto it = _handler.find(ses->fd_);
	if(_handler.end() == it)
	{
		update_event(EPOLL_CTL_ADD, ses->_fd, event);
		_handler.insert(std::make_pair(ses->fd, ses));
	}
	else
	{
		update_event(EPOLL_CTL_MOD, ses->_fd, event);
	}	
}

bool event_loop::unregister_event(std::shared_ptr<session> ses, int event)
{
	std::lock_guard<std::mutex> guard(_lck_handler);
	auto it = _handler.find(ses->fd_);
	if(_handler.end() != it)
	{
		update_event(EPOLL_CTL_DEL, ses->_fd, event);
		_handler.erase(it);
	}
}

bool update_event(int op, int fd, int event_opt)
{
	struct epoll_event event;
	bzero(&event, sizeof(event));
	event.events = event_opt;
	epoll_ctl(_event_fd, op, fd, &event);
}