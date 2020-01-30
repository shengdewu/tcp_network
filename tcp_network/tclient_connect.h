#include "tasync_connect.h"
#include  "public/active_thread.h"

class tclient_connect : public tasync_connect ,std::enable_shared_from_this<tclient_connect>
{
public:
    tclient_connect(event_loop *loop) : tasync_connect(loop){}

    bool connect_server(std::string ip, unsigned int port);

public:
    virtual  void handle_create(){}
    virtual  void handle_close(){}
    virtual  void send_heart();

private:
    void reconenct();
    bool connect_server();
    void keep_alive();

    std::string _ip;
    unsigned int _port;
    std::shared_ptr<active_thread> _probe;
};