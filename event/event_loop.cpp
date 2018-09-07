#include "event_loop.h"
#include <sys/socket.h>
#include "_active_thread.h"
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <errno.h>
#include "event_handler.h"
#include <string.h>

#include <iostream>

#define MAX_EVENTS_SIZE 10

event_loop::event_loop(int max_thread)
	:_event_fd(epoll_create(EPOLL_CLOEXEC))
{
	for(int i =0; i<max_thread; ++i)
	{
		_active_thread.emplace_back(std::make_shared<active_thread>(std::bind(&event_loop::loop, this)));
	}

	init_delete_epoll();
}


event_loop::~event_loop()
{
	delete_epoll();
	_active_thread.clear();
	close(_event_fd);
}

void event_loop::loop()
{
	struct epoll_event events[MAX_EVENTS_SIZE] = {0};
	int nfd = epoll_wait(_event_fd, &events, MAX_EVENTS_SIZE, -1);
	if(nfd > 0)
	{
		for(int i=0; i<nfd; ++i)
		{
			shared_ptr<event_handler> handler = find_handler(events[i].data.fd);
			if(handler)
			{
				int events = events[i].events;
				if(events & EPOLLIN)
				{
					handler->notify_read_event();
				}

				if(events & EPOLLOUT)
				{
					handler->notify_write_event();
				}

				if(!(events & EPOLLIN) && (events & EPOLLERR || events & EPOLLHUP))
				{
					handler->notify_error_event();
				}
			}
		}
	}
}

bool event_loop::register_event(int fd, std::shared_ptr<event_handler> handler)
{
	auto_rwlock guard(&_lck_handler);
	auto it = _handler.find(fd);
	if(_handler.end() == it)
	{
		update_event(EPOLL_CTL_ADD, fd, EPOLLIN);
		_handler.insert(std::make_pair(fd, handler));
	}
}

bool event_loop::delete_handler(int fd);
{
	auto_rwlock guard(&_lck_handler);
	auto it = _handler.find(fd);
	if(_handler.end() != it)
	{
		update_event(EPOLL_CTL_DEL, fd, 0);
		_handler.erase(it);
	}
}

bool event_loop::update_event(int op, int fd, int events, bool once )
{
	struct epoll_event event;
	bzero(&event, sizeof(event));
	event.events = events;
	if(once)
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
	int one = 1;
	if(sizeof(one) != send(_wake_up_fd, &one, siezof(one)))
	{
		std::cout << "delete_epoll failed" << errno << std::endl;
	}
}

std::shared_ptr<event_handler> event_loop::find_handler(int fd)
{
	shared_ptr<event_handler> handler;
	{
		auto_rwlock guard(&_lck_handler, false);
		auto it = _handler.find(fd);
		handler = *it;
	}

	return handler;
}