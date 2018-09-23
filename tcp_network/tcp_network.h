#pragma once 
#include <memory>
#include <map>
#include "tconnect.h"
#include "public/rwlock.h"

class tcp_listen;
class event_loop;
class tserver_connect;
class tcp_network
{
public:
    tcp_network();

    bool init_network(std::string ip, int port);

    void delete_connect(int fd);

protected:
    virtual std::shared_ptr<tserver_connect> create_connect(int fd) = 0;

private:
    void handle_new_accept(int fd);
    
private:
    std::shared_ptr<event_loop> event_loop_;
    std::shared_ptr<tcp_listen> tcp_listen_;

    std::map<int, std::shared_ptr<tserver_connect>> con_map_; //<socket_fd, connect>

    rwlock con_lck_;
};