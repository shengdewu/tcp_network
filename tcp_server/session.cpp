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

void session::send(std::shared_ptr<char> data, unsigned int size)
{
    data_unit udata(data, size);
    _data_mtx.lock();
    _data_pool.emplace_back(udata);
    _data_mtx.unlock();
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
    if(!_data_pool.empty())
    {
        _data_mtx.lock();
        data_unit udata = _data_pool.front();
        _data_pool.pop_front();
        _data_mtx.unlock();

        std::shared_ptr<char> data = udata.data();
        unsigned int length = udata.length();
        if(length > 0)
        {
            int nsended = 0;
            while(nsended != length)
            {
                int nsend = write(_fd, data.get() + nsended, length - nsended);
                nsended += nsend;
            }
        }
        
    }
    
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
