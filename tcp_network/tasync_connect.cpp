#include "tasync_connect.h"
#include <errno.h>
#include "public/log_file.h"


int tasync_connect::send_msg(const char *pdata, unsigned int len)
{
    std::lock_guard<std::mutex> lock(_data_lck);
    if(!_data_buf.empty())
    {
        _data_buf.push_back(std::make_shared<data_unit>(pdata, len));
        return 0;
    }

    int sendl = tconnect::send_data(pdata, len);

    if(sendl > 0 && sendl < len)
    {
        _data_buf.emplace_back(std::make_shared<data_unit>(pdata, len, sendl));
        LOG_FILE(LOGI_LVL::LOGI_ALL, "the socket send delay [%d]-[%d]\n", _fd, _data_buf.size());
        post_write_event(_fd);
        return sendl;      
    }

    delete[] pdata;
    pdata = nullptr;        

    return sendl;
}

bool tasync_connect::handle_write_event()
{
    std::lock_guard<std::mutex> lock(_data_lck);
    while(!_data_buf.empty())
    {
        std::shared_ptr<data_unit> data = _data_buf[0];
        int sendl = tconnect::send_data(data.data_buf + data.send_len, data.buf_len - data.send_len);
        if(sendl + data.send_len == data.buf_len) //数据发送完毕
        {
            _data_buf.pop_front();
        }
        else if (sendl > 0)
        {
            data.send_len += sendl;
        }
        else //发送失败
        {
            return false;
        }
    }

    return true;
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