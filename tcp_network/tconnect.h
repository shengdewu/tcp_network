#pragma once 

class tconnect
{
public:
    tconnect(){}

    virtual bool send_msg(char *data, unsigned int length);

protected:
    virtual  void handle_network_data(char *data, unsigned int length) = 0;
    virtual  void handle_create() = 0;
    virtual  void handle_close() = 0;
};