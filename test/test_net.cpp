#include "tcp_server.h"
#include "session.h"
#include <iostream>

class client_session : public session
{
public:
    client_session(int fd):session(fd)
    {

    }
private:

	virtual bool handle_read_event(char *data, unsigned int size) override 
    {
        std::cout << __FILE__ << ", " << __LINE__ << std::string(data, size) << std::endl;
        return false;
    }

};

class tcp_network : public tcp_server
{
protected:
    virtual std::shared_ptr<session> create_session(int fd) override
    {
        return std::make_shared<client_session>(fd);
    }
};

int main(int argc, char const *argv[])
{
    std::shared_ptr<tcp_network> server = std::make_shared<tcp_network>();
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
