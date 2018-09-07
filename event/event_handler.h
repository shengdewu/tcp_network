#pragma once
#include <memory>
#include "event_loop.h"
#include <actomic>
class event_handler
{
public:
    event_handler(event_loop *loop, int efd):loop_(loop),efd_(efd),whandle_(false),rhandle_(true){}

    //通知事件
    inline void notify_write_event();
    inline void notify_read_event();
    inline void notify_error_event(){ handle_error_event(); };

protected:
    //处理通知事件
    virtual bool handle_write_event() = 0;
    virtual bool handle_read_event() = 0;
    virtual void handle_error_event() = 0;

private:
    //投递读写事件
    inline void post_write_event();
    inline void post_read_event();
    
private:
    event_loop *loop_;
    int efd_;
    int events_;
}

//
//inline
//

inline void event_handler::notify_write_event()
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

inline void event_handler::notify_read_event()
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

inline void event_handler::post_write_event()
{
    events_ |= EPOLLOUT;
    loop_->update_event(EPOLL_CTL_MOD, efd_, events_);
    events_ &= ~EPOLLOUT;
}

inline void event_handler::post_read_event()
{
    events_ |= EPOLLIN;
    loop_->update_event(EPOLL_CTL_MOD, efd_, events_);
    events_ &= ~EPOLLIN;
}
