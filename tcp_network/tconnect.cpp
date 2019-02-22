#include "tconnect.h"

void tconnect::close_connect()
{
    if(_fd > 0)    
    {
        shutdown(_fd, SHUT_RDWR);
        close(_fd);
    }
}

void tconnect::set_noblock()
{
    int flags = fcntl(_fd, F_GETFL, 0);
    fcntl(_fd, F_SETFL, flags|O_NONBLOCK);   
}