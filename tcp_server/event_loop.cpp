#include "event_loop.h"
#include <sys/socket.h>
#include "_active_thread.h"
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <errno.h>
#include "session.h"
#include <string.h>

#include <iostream>

#define MAX_EVENTS_SIZE 32

event_loop::event_loop(int max_thread)
	:_event_fd(epoll_create(EPOLL_CLOEXEC)),
	_exit(false)
{
	for(int i =0; i<max_thread; ++i)
	{
		_active_thread.emplace_back(std::make_shared<active_thread>(std::bind(&event_loop::loop, this)));
	}

	init_delete_epoll();
}


event_loop::~event_loop()
{
	//wake up wait
	_active_thread.clear();
}

void event_loop::loop()
{
	std::vector<struct epoll_event> events(MAX_EVENTS_SIZE);
	int nfd = epoll_wait(_event_fd, &*events.begin(), events.size(), 10000);
	if(nfd > 0)
	{
		for(int i=0; i<nfd; ++i)
		{
			auto it = _handler.find(events[i].data.fd);
			if(it == _handler.end())
			{
				continue;
			}

			switch (events[i].events)
			{
				case EPOLLIN:
					it->second->notify_read_event();
				break;
				case EPOLLOUT:
					it->second->notify_write_event();
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

bool event_loop::register_event(std::shared_ptr<session> ses, int event)
{
	std::lock_guard<std::mutex> guard(_lck_handler);
	auto it = _handler.find(ses->fd());
	if(_handler.end() == it)
	{
		update_event(EPOLL_CTL_ADD, ses->fd(), event);
		_handler.insert(std::make_pair(ses->fd(), ses));
	}
	else
	{
		update_event(EPOLL_CTL_MOD, ses->fd(), event);
	}	
}

bool event_loop::unregister_event(std::shared_ptr<session> ses, int event)
{
	std::lock_guard<std::mutex> guard(_lck_handler);
	auto it = _handler.find(ses->fd());
	if(_handler.end() != it)
	{
		update_event(EPOLL_CTL_DEL, ses->fd(), event);
		_handler.erase(it);
	}
}

bool event_loop::update_event(int op, int fd, int event_opt, bool once )
{
	struct epoll_event event;
	bzero(&event, sizeof(event));
	event.events = event_opt;
	if(bool)
	{
		event.events |= (EPOLLET | EPOLLONESHOT);
	}
	epoll_ctl(_event_fd, op, fd, &event);
}

bool event_loop::init_delete_epoll()
{
	_wake_up_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if(_wake_up_fd < 0)
	{
		std::cout << "init_delete_epoll::loop" << errno << std::endl;
		return false;
	}
	update_event(EPOLL_CTL_ADD, _wake_up_fd, EPOLLIN, false);

	return true;
}

void event_loop::delete_epoll()
{
	//write(_wake_up_fd, )
}