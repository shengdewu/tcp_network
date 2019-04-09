#include "tconnect.h"
#include "event/event_handler.h"
#include <deque>
#include <mutex>

class tasync_connect : public tconnect, public event_handler
{
public:
    tasync_connect(event_loop *loop):event_handler(loop){}
    
    //0  入队列 > 0 发送的字节 < 0 失败
    int send_msg(const char *pdata, unsigned int len);
protected:
    //网络消息
    virtual  void handle_create(){}
    virtual  void handle_close(){}  
    void resend_data();
    
private:
    //处理通知事件
    virtual  bool handle_write_event() override final;
    virtual bool handle_read_event() override final;
    virtual void handle_error_event() override final;

private:
    std::deque<std::shared_ptr<data_unit>> _data_buf;
    std::mutex  _data_lck;
};