#include "session.h"
#include <unistd.h>

session::session(int fd, tcp_server *tcp_server, event_loop *event)
:_fd(fd),
_tcp_server(tcp_server),
_event_loop(event)
{
}


session::~session()
{
}

void session::send(std::string data, unsigned int size)
{
    post_write_event();
}

void session::notify_read_event()
{
    char data[1024];
    int size = read(_fd, data, sizeof(data));

    if(size > 0)
    {
        handle_read_event(data, size);
    }
    else
    {
        if(size == 0 || errno != EAGAIN)
        {
            
        }
    }
    
}

void session::notify_write_event()
{
    char data[1024];
    int size = write(_fd, data, sizeof(data));
}


void session::post_read_event()
{
	int event = EPOLLIN;
   
    _event_loop->update_event(EPOLL_CTL_MOD, _fd, event)
}

void session::post_write_event()
{
	int event = EPOLLOUT;
   
    _event_loop->update_event(EPOLL_CTL_MOD, _fd, event)
}
