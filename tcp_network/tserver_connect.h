#pragma once
#include "public/event_handler.h"
#include "tasync_connect.h"

class tserver_connect : public tasync_connect
{
public:
    tserver_connect(int fd,event_loop *loop):tasync_connect(loop){ _fd = fd; }
};