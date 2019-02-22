#include "tconnect.h"
#include "event/event_handler.h"
#include "trecv_buf.h"

class tasync_connect : public tconnect, public event_handler
{
public:
    tasync_connect(event_loop *loop):event_handler(loop){}
    
protected:
    //网络消息
    virtual  void handle_create(){}
    virtual  void handle_close(){}  

private:
    //处理通知事件
    virtual  bool handle_write_event() override final;
    virtual bool handle_read_event() override final;
    virtual void handle_error_event() override final;
};