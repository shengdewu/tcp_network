#pragma once
#include <thread>
#include <functional>
#include <deque>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <memory>

class thread_pool
{
public:
	typedef std::function<void(void)> thread_func;
public:
	thread_pool(unsigned int thread_num = 4);

	~thread_pool();

	void post(thread_func func);

private:
	thread_pool(const thread_pool &tp) = delete;
	const &thread_pool operator = (const thread_pool &tp) = delete;

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

