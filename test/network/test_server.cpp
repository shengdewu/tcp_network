#include "tcp_network.h"
#include "tserver_connect.h"
#include "public/log_file.h"
#include <iostream>

class server_session : public tserver_connect
{
public:
    server_session(int fd):tserver_connect(fd){  }
private:

	virtual bool process_unit_data(const char *data, unsigned int length) override 
    {
        LOG_FILE(LOGI_LVL::LOGI_DEBUG, "the server recv [%s]\n", std::string(data, size).c_str());

        send_msg(data, length);
        
        return false;
    }

};

class tcp_server : public tcp_network
{
public:
    tcp_server(){ _loop = new event_loop(8); }

protected:
    virtual std::shared_ptr<session> create_connect(int fd) override
    {
        return std::make_shared<server_session>(fd, _loop);
    }

private:
    event_loop *_loop;
};

int main(int argc, char const *argv[])
{
    std::shared_ptr<tcp_server> server = std::make_shared<tcp_server>();
    server->init_tcp("0.0.0.0", 12345);

    std::cout << "wait exit.."<<std::endl;
    char c = '\0';
    std::cin >> c;
    while('e' != c)
    {
        std::cin >> c;
    }

    return 0;
}
