#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "tcp_listen.h"
#include "public/active_thread.h"
#include "public/log_file.h"

tcp_listen::tcp_listen(notify_new_connect func)
:notify_func_(func)
{ 
}

tcp_listen::~tcp_listen()
{
    close(listen_fd_);
}

bool tcp_listen::start(std::string ip, int port)
{
    do
    {
        listen_fd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_CP);
        if(listen_fd_ < 0)
        {
            break;
        }

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(struct sockaddr_in));
        if( 1 != inet_pton(AF_INET, ip.c_str(), (void*)(&addr.sin_addr)))
        {
            break;
        }    
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        //bind
        if(!bind(listen_fd_, (struct sockaddr*)(&addr),sizeof(addr)))
        {
            break;
        }
        //listen
        if(!listen(listen_fd_, 5))
        {
            break;
        }

        listen_thread_= std::make_shared<active_thread>(std::bind(&tcp_listen::listen_loop, this));

        return true;
    }while(false);

    LOG_FILE(LOGI_LVL::LOGI_ERROR, "network api faild[%s]\n", std::strerror(errno).c_str());
    return false;
}

void tcp_listen::listen_loop()
{
    //accept
    struct sockaddr_in client_addr;
    int len = sizeof(client_addr);
    int client_fd = accept(_listen_fd, (struct sockaddr*)(&client_addr),(socklen_t*)(&len));
    if(client_fd < 0) 
    {
        continue;
    }

    set_noblock(client_fd);

    char client_ip[INET_ADDRSTRLEN] ={'\0'};
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));

    LOG_FILE(LOGI_LVL::LOGI_INFO, "accept[%s_%d]connected\n", client_ip, client_addr.sin_port);
        
    notify_func_(client_fd);  
}