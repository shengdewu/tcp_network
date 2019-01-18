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
	active_thread(thread_func func);

	~active_thread();

private:
	active_thread(const active_thread &tp) = delete;
	const active_thread& operator = (const active_thread &tp) = delete;

private:
	void loop();

private:
	thread_func _func;
	std::shared_ptr<std::thread> _thread;
	bool		_exit;
};

