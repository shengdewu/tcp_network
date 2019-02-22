#include <stdlib.h>
#include "public/log_file.h"

class trecv_buf
{
public:
    trecv_buf():_size(MAX_BUF_LEN),_data_size(0),_head(nullptr),_data_head(nullptr){ realloc_buf(); }
    ~trecv_buf(){ free(_head); _head = nullptr; _data_head=nullptr; }

    inline bool write_data(const char *data, unsigned int size);

    inline bool get_data(char *data, unsigned int &size);

private:
    inline bool realloc_buf();
    inline bool empty();

private:
    const unsigned int MAX_BUF_LEN = 4 * 1024 * 1024;
    const unsigned int WARN_BUF_LEN = 64 * 1024 * 1024;
    unsigned int _size; //缓冲区大小
    unsigned int _data_size; //数据大小
    char *_head;  //缓冲区头
    char *_data_head; //数据头
};

//
//inline
//
bool trecv_buf::write_data(const char *data, unsigned int size)
{
    if(_size - _data_size <= size) //剩余空间不足
    {
        if (_data_head - _head + _size - _data_size > size) //剩余可用空间
        {
            int valid = _data_size - (_data_head - _head);
            memmove(_head, _data_head, valid);
            _data_size = valid;
            _data_head = _head;
        }
        else
        {
            return realloc_buf() && write_data(data, size); //size 可能需要多次分配才能满足
        }
    }

    memcpy(_head + _data_size, data, size);
    _data_size += size;

    return true;
}

void trecv_buf::get_data(char *&data, unsigned int &size)
{
    size = 0;
    data = nullptr;
    if(!empty())
    {
        data = _data_head;
        size = _data_size - (_data_head - _head);

        _data_head += size;
    }
}

bool trecv_buf::realloc_buf()
{
    if(_size >= WARN_BUF_LEN)
    {
        LOG(LOGI_LVL::LOGI_ERROR, "当前缓冲区大小已达上线，禁止分配[%d-->%d]\n", _size, WARN_BUF_LEN);
        return false;
    }

    int offset = _data_head - _head;
    _size = _size << 1;
    _head = (char*)realloc(_head, _size);
    _data_head = _head + offset;

    return true;
}

bool trecv_buf::empty()
{
    return (_data_size - (_data_head - _head) <= 0);
}
