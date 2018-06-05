#pragma once
#include <thread>
#include <functional>
#include <deque>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <memory>

class active_thread
{
public:
	typedef std::function<void(void)> thread_func;
public:
	active_thread(unsigned int thread_num = 4);

	~active_thread();

	active_thread(const active_thread & at) = delete;

	void post(thread_func func);

private:
	void loop();

private:
	std::vector<std::shared_ptr<std::thread>> _thread;
	std::deque<thread_func> _task_queue;
	std::mutex				_task_mtx;
	std::mutex				_condition_mtx;
	std::condition_variable	_condition;
	bool					_exit;
};

