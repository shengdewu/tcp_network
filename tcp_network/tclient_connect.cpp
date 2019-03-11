#include "tclient_connect.h"
#include <strings.h>
#include "public/log_file.h"


bool tclient_connect::connect_server(std::string ip, unsigned int port)
{
    _ip = ip;
    _port = port;
    return connect_server();
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

    handle_create();
    
    return true;
}