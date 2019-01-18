#include "thread_pool.h"


thread_pool::thread_pool(unsigned int thread_num)
: _exit(false)
{
	for (unsigned int i = 0; i < thread_num; ++i)
	{
		_thread.emplace_back(std::make_shared<std::thread>(std::bind(&thread_pool::loop, this)));
	}
}


thread_pool::~thread_pool()
{
	_exit = true;
	_condition.notify_all();

	for (unsigned int i = 0; i < _thread.size(); ++i)
	{
		_thread.at(i)->join();
	}
}

void thread_pool::post(thread_func func)
{
	std::lock_guard<std::mutex> guard(_task_mtx);
	_task_queue.emplace_back(func);
	_condition.notify_one();
}


void thread_pool::loop()
{
	while (!_exit)
	{
		std::unique_lock<std::mutex> lk(_condition_mtx);
		_condition.wait(lk, [this]{ return  !_task_queue.empty(); });

		thread_func func = nullptr;
		{
			std::lock_guard<std::mutex> guard((_task_mtx));
			func = std::move(_task_queue.front());
			_task_queue.pop_front();
		}

		if (func)
		{
			func();
		}
	}
}