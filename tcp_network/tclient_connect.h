#include "tasync_connect.h"

class tclient_connect : public tasync_connect
{
public:
    tclient_connect(event_loop *loop) : tasync_connect(loop){}

    bool connect_server(const char *ip, unsigned int port);

public:
    virtual  bool process_unit_data(const char *data, unsigned int length) override final;
    virtual  void handle_create();
    virtual  void handle_close();

private:
    bool reconenct();
};