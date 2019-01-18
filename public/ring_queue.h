#pragma once
/*
名称:环形队列
说明:队列大小必须是2的倍数,加入信号量
*/
#include <condition_variable>
#include <mutex>
#include <memory>
#include "log_file.h"

#define RING_MAX_SIZE (1024 * 1024 * 4)

template<class T>
class ring_queue
{
public:
    ring_queue(std::string key):_phead(nullptr),_front(0), _tail(0), _size(0),_is_live(false),_key(key){ reallocate(); }
    ~ring_queue(){ delete []_phead; }

    inline std::shared_ptr<T> pop();
    inline void push(const std::shared_ptr<T> element);
    inline void drop_out();

private:
    inline bool empty();
    inline bool full();
    inline void reallocate();

private:
    std::shared_ptr<T> *_phead; //数组头指针
    unsigned int _front;
    unsigned int _tail;
    unsigned int _size; // > 0
    bool _is_live;
    std::string _key;
    std::condition_variable _wait;
    std::mutex _wmutx;
    std::mutex _rmutx;
};


//
//inline
//
template<class T>
bool ring_queue<T>::empty()
{
    return _front == _tail;
}

template<class T>
bool ring_queue<T>::full()
{
    return _tail == ((_size - 1) & (_front+1));
}

template<class T>
std::shared_ptr<T> ring_queue<T>::pop()
{
    std::unique_lock<std::mutex> lk(_rmutx);
    _wait.wait(lk, [this]{ return !_is_live && !empty(); });

    std::shared_ptr<T> element = _phead[_tail];
    _tail = ((_tail + 1) & (_size - 1));

    return element;
}

template<class T>
void ring_queue<T>::push(const std::shared_ptr<T> element)
{
    std::unique_lock<std::mutex> lk(_wmutx);
    if(full())
    {
        std::unique_lock<std::mutex> lk(_rmutx);
        reallocate();
    }

    _phead[_front] = element;
    _front = ((_front + 1) & (_size - 1));

    _wait.notify_all();
}

template<class T>
void ring_queue<T>::drop_out()
{
    _is_live = false;
    _wait.notify_all();
}

template<class T>
void ring_queue<T>::reallocate()
{
    _size = _size == 0 ? RING_MAX_SIZE :_size << 1;
    std::shared_ptr<T> *pnewhead = new std::shared_ptr<T>[_size];
    unsigned int index = 0;
    while(_tail != _front)
    {
        pnewhead[index] = _phead[_tail];
        _tail= ((_tail + 1 ) & (_size - 1));
        ++index;
    }

    _tail = 0;
    _front = index;

    if(_phead)
    {
        delete [] _phead;
    }
    _phead = pnewhead;

    if(_size > 2 * RING_MAX_SIZE )
    {
        LOG(LOGI_LVL::LOGI_WARN, "队列过大:%s-%d\n", _key.c_str(), _front - _tail); 
    }
}




         
         
         
         