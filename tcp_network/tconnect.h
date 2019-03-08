#pragma once 
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

class data_unit
{
public:
    data_unit(char *buf, unsigned int len, unsigned int send=0)data_buf(pbuf),buf_len(len),send_len(send){}
    ~data_unit(){ delete [] buf; }
    char *data_buf;
    unsigned int buf_len;
    unsigned int send_len;

};
class tconnect
{
public:
    tconnect()_fd(-1){}
    ~tconnect(){}

    // > 0 发送字节 < 0 发送失败
    int send_data(const char *data, unsigned int length);

    void close_connect();
    void set_noblock();

protected:
    virtual  bool process_unit_data(const char *data, unsigned int length) = 0;
    virtual  void handle_create() = 0;
    virtual  void handle_close() = 0;
    bool  recv_data();

protected:
    int _fd;
};

