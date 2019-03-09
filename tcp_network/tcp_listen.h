#pragma once
#include <memory>
#include <string>
#include <functional>

class active_thread;
class tcp_listen
{
public:
    typedef std::function<void(int)>  notify_new_connect;
public:
    tcp_listen(notify_new_connect func);
    ~tcp_listen();

    bool start(std::string ip, int port);

private:
    void listen_loop();

private:
    int listen_fd_;
    notify_new_connect  notify_func_;
    std::shared_ptr<active_thread> listen_thread_;
};