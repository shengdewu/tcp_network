#include "active_thread.h"


active_thread::active_thread(unsigned int thread_num)
: _exit(false)
{
	for (auto i = 0; i < thread_num; ++i)
	{
		_thread.emplace_back(std::make_shared<std::thread>(std::bind(&active_thread::loop, this)));
	}
}


active_thread::~active_thread()
{
	_exit = true;
	_condition.notify_all();

	for (auto i = 0; i < _thread.size(); ++i)
	{
		_thread.at(i)->join();
	}
}

void active_thread::post(thread_func func)
{
	std::lock_guard<std::mutex> guard(_task_mtx);
	_task_queue.emplace_back(func);
	_condition.notify_one();
}


void active_thread::loop()
{
	while (_exit)
	{
		if (_task_queue.empty())
		{
			std::unique_lock<std::mutex> lc(_condition_mtx);
			_condition.wait(lc);
			continue;
		}

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