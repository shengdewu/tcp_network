#include "tasync_connect.h"
#include <errno.h>
#include "public/log_file.h"

bool tasync_connect::handle_write_event()
{

}

bool tasync_connect::handle_read_event()
{
    int buf_len = 8192;
    const char *buf[buf_len] = {'\0'};

    int len = recv(_fd, buf, buf_len, MSG_NOSIGNAL);

    if(len > 0)
    {
        handle_network_data(buf, buf_len);
    }
    else 
    {
        if (len <= 0)
        {
            if(errno==EAGAIN || errno == EWOULDBLOCK)
            {
                return true;
            }

            return true;
        }
    }

    return true;
}

void tasync_connect::handle_error_event()
{
    LOG_FILE(LOGI_LVL::LOGI_ERROR, "socket[%d] is closed:[%s]\n", _fd, std::strerror(errno).c_str());
    close_connect();
    loop_->delete_handler(_fd);
}