#include "session.h"
#include <unistd.h>

session::session(int fd)
:_fd(fd)
{
}


session::~session()
{
}

void session::send(std::shared_ptr<char*> data, unsigned int size)
{

}

void session::notify_read_event()
{
    char data[1024];
    int size = read(_fd, data, sizeof(data));

    handle_read_event(data, size);
}

void session::notify_write_event()
{
    char data[1024];
    int size = write(_fd, data, sizeof(data));
}

void session::post_read_event()
{

}

void session::post_write_event()
{

}
