#pragma once
#include <memory>

class event_loop;
class event_handler
{
public:
    event_handler(std::shared_ptr<event_loop> loop);

    virtual void notify_write_event() = 0;
    virtual void notify_read_event() = 0;

    virtual void post_write_event();
    virtual void post_read_event();
    
private:
    std::shared_ptr<event_loop> loop_;
}