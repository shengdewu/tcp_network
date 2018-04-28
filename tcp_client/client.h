#pragma once

class client
{
public:
    bool init_client();
    bool send_data_async();
    bool send_data_sync();
};