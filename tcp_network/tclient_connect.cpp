#include "tclient_connect.h"
#include <strings.h>
#include "public/log_file.h"
#include <thread>
#include <chrono>

bool tclient_connect::connect_server(std::string ip, unsigned int port)
{
    _ip = ip;
    _port = port;
    bool flag = connect_server();

    std::thread t(&tclient_connect::reconenct, this);
    t.detach();

    _probe = std::make_shared<active_thread>(std::bind(&tclient_connect::keep_alive, this));

    return flag;
}

bool tclient_connect::connect_server()
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == _fd)
    {
        LOG_FILE(LOGI_LVL::LOGI_ERROR, "the socket create faild:[%s]\n", std::strerror(errno));
        return false;
    }

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(_port);
    inet_pton(AF_INET, _ip.c_str(), servaddr.sin_addr);

    if-1 == connect(_fd, &servaddr, sizeof(servaddr)))
    {
        LOG_FILE(LOGI_LVL::LOGI_ERROR, "the socket connect faild:[%s]\n", std::strerror(errno));
        return false;
    }

    set_noblock();

    handle_create();

    _loop->register_event(_fd, this);
    
    LOG(LOGI_LVL::LOGI_INFO, "连接服务器成功:[%s：%d-%d]\n", _ip.c_str(), _port, _fd);

    _running = true;

    return true;
}

void tclient_connect::reconenct()
{
    while(!_running && -1 == _fd)
    {
        LOG(LOGI_LVL::LOGI_WARN, "尝试重连服务器:[%s：%d]\n", _ip.c_str(), _port);
        if(connect_server())
        {
            resend_data();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void tclient_connect::send_heart()
{
    std::string content = "heart";

    send_data(content.c_str(), content.length());
}

void tclient_connect::keep_alive()
{
    time_t diff = std::time(nullptr) - _send_time;
    if(_fd > 0)
    {
        if(diff > 60 * 10)
        {
            LOG(LOGI_LVL::LOGI_WARN, "服务器异常断开:[%s：%d-%d]\n", _ip.c_str(), _port, _fd);
            handle_error_event();
        }
        else if(diff > 3 * 60)
        {
            send_heart();
        }
        else
        {
            LOG_FILE(LOGI_LVL::LOGI_INFO, "心跳检测触发:[%s：%d-%d]\n", _ip.c_str(), _port, _fd);
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(20));
}

