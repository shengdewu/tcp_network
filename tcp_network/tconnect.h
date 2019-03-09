#pragma once 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

class tconnect
{
public:
    tconnect(int fd)_socket_fd(fd);
    ~tconnect();

    virtual bool send_msg(char *data, unsigned int length);

    inline void close();

protected:
    virtual  void handle_network_data(char *data, unsigned int length) = 0;
    virtual  void handle_create() = 0;
    virtual  void handle_close() = 0;

private:
    int _socket_fd;
};

//
//inline
//
void tconnect::close()
{
    ::close(_socket_fd);
}