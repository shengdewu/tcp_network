#include "active_thread.h"


active_thread::active_thread(thread_func func)
:_func(func),
_exit(false)
{
	_thread = std::make_shared<std::thread>(std::bind(&active_thread::loop, this));
}


active_thread::~active_thread()
{
	_exit = true;
	if(_thread->joinable())
	{
		_thread->join();
	}
}

void active_thread::loop()
{
	while (!_exit)
	{
		_func();
	}
}