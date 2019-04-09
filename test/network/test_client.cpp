
#include "tclient_connect.h"
#include "public/log_file.h"
#include <iostream>

class client_session : public tclient_connect
{
public:
    client_session(event_loop *loop=new event_loop(2)):tclient_connect(loop)；_loop(loop){ }
    ~client_session( delete _loop; )

private:

	virtual bool process_unit_data(const char *data, unsigned int length) override 
    {
        LOG_FILE(LOGI_LVL::LOGI_DEBUG, "the client recv [%s]\n", std::string(data, size).c_str());

        send_msg(data, length);
        
        return true;
    }

    event_loop *_loop;
};

int main(int argc, char const *argv[])
{
    std::shared_ptr<client_session> client = std::make_shared<client_session>();
    client->connect_server("0.0.0.0", 12345);

    std::string data = "baoyong";
    client->send_msg(data.c_str(), data.length());

    std::cout << "wait exit.."<<std::endl;
    char c = '\0';
    std::cin >> c;
    while('e' != c)
    {
        client->close_connect();
        std::cin >> c;
    }

    return 0;
}
