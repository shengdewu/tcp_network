#include "tconnect.h"
#include "public/log_file.h"

int tconnect::send_data(const char *data, unsigned int length)
{
    int remain = length;
    while (remain > 0)
    {
        int sendl = send(_fd, data + (length - remain), remain, MSG_NOSIGNAL);
        if(sendl > 0)
        {
            remain -= sendl;
            _send_time = std::time(nullptr);
        }
        else
        {
            if(errno != EAGAIN)
            {
                LOG_FILE(LOGI_LVL::LOGI_ERROR, "the socket send failed [%d]-[%s]\n", fd_, strerror(errno));
                _send_time = 0;
                return -1;
            }

            return len - remain;
        }
    } 

    return remain;
}

bool tconnect::recv_data()
{
    const static int buf_len = 8192; // 8 * 1024
    while(true)
    {
        const char *buf[buf_len] = {'\0'};

        int len = recv(_fd, buf, buf_len, MSG_NOSIGNAL);

        if(len > 0)
        {
            //是完整得一个单元数据, 则继续接收数据
            if(process_unit_data(buf, len))
            {
                continue;
            }

            return true;
        }
        else 
        {
            if (len != 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
            {
                return true;
            }
            else 
            {
                LOG_FILE(LOGI_LVL::LOGI_ERROR, "the socket recv failed [%d]-[%s]\n", fd_, strerror(errno));
                return false;
            }
        }

        return true;  
    }

}

void tconnect::shutdown_connect()
{
    shutdown(_fd, SHUT_RDWR);
}

void tconnect::close_connect()
{
    shutdown(_fd, SHUT_RDWR);
    close(_fd);
    _running = false;
}

void tconnect::set_noblock()
{
    int flags = fcntl(_fd, F_GETFL, 0);
    fcntl(_fd, F_SETFL, flags|O_NONBLOCK);   
}