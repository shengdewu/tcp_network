#include "tcp_network.h"
#include "event/event_loop.h"
#include "tcp_listen.h"

tcp_network::tcp_network()
:event_loop_(std::make_shared<event_loop>()),
 tcp_listen_(std::make_shared<tcp_listen>(std::bind(&tcp_network::handle_new_accept, this)))
{
}

bool tcp_network::init_network(std::string ip, int port)
{
    return tcp_listen_->start(ip, port);
}

void tcp::network::delete_connect(int fd)
{
    std::shared_ptr<tserver_connect> con;
    {
        auto_rwlock lck(&con_lck_, false);
        auto it = con_map_.find(fd);
        if(it != con_map_.end())
        {
            con =  it.second;
            con_map_.erase(it);
        }
    }

    con->handle_close();
}

void tcp_network::handle_new_accept(int fd)
{
    std::shared_ptr<tserver_connect> con = create_connect(fd);
    {
        auto_rwlock lck(&con_lck_);
        con_map_[fd] = con;
    }

    event_loop_->register_event(fd, con);

    con->handle_create();
}
