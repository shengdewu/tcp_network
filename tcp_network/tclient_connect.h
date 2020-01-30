#include "tasync_connect.h"

class tclient_connect : public tasync_connect
{
public:
    tclient_connect(event_loop *loop) : tasync_connect(loop){}

    bool connect_server(std::string ip, unsigned int port);

public:
    virtual  void handle_create(){}
    virtual  void handle_close(){}

private:
    bool reconenct();
    bool connect_server();
    std::string _ip;
    unsigned int _port;
};