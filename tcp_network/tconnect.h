#pragma once 
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
class tconnect
{
public:
    tconnect()_fd(-1){}
    ~tconnect(){}

    virtual bool send_msg(char *data, unsigned int length);

    void close_connect();
    void set_noblock();

protected:
    virtual  void handle_network_data(const char *data, unsigned int length) = 0;
    virtual  void handle_create() = 0;
    virtual  void handle_close() = 0;

protected:
    int _fd;
};

