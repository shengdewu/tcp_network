#pragma once
#include <memory>
#include "event_loop.h"
#include <actomic>
class event_handler
{
public:
    event_handler(event_loop *loop):_loop(loop){}

    //通知事件
    inline void notify_write_event(int fd);
    inline void notify_read_event(int fd);
    inline void notify_error_event(){ handle_error_event(); };

protected:
    //处理通知事件
    virtual bool handle_write_event() = 0;
    virtual bool handle_read_event() = 0;
    virtual void handle_error_event() = 0;
    //投递读写事件
    inline void post_write_event(int fd);
    inline void post_read_event(int fd);
    
protected:
    event_loop *_loop;
private:
    atomic<int> _events;
}

//
//inline
//
inline void event_handler::notify_write_event(int fd)
{
    if(handle_write_event())
    {
        post_write_event();
    }
    else
    {
        handle_error_event();
    }
}

inline void event_handler::notify_read_event(int fd)
{
    if(handle_read_event())
    {
        post_read_event();
    }
    else
    {
        handle_error_event();
    }
}

inline void event_handler::post_write_event(int fd)
{
    _events |= EPOLLOUT;
    int t_event = _events;
    _loop->update_event(EPOLL_CTL_MOD, fd, t_event);
    _events &= ~EPOLLOUT;
    
}

inline void event_handler::post_read_event(int fd)
{
    _events |= EPOLLIN;
    int t_event = _events;
    _loop->update_event(EPOLL_CTL_MOD, fd, t_event);
    _events &= ~EPOLLIN;
    
}
