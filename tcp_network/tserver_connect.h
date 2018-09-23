#pragma once
#include "public/event_handler.h"
#include "tconnect.h"

class tserver_connect : public event_andler, public tconnect
{
protected:
    //处理通知事件
    virtual bool handle_write_event();
    virtual bool handle_read_event();
    virtual void handle_error_event();
    //
    virtual  void handle_network_data(char *data, unsigned int length) = 0;
    virtual  void handle_create(){}
    virtual  void handle_close(){}    
};