#pragma once
#include <string>

class client
{
public:
    bool init_client(std::string ip, );
    bool send_data_async();
    bool send_data_sync();
};