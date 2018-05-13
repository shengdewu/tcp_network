#pragma once
#include <map>
#include <memory>

class session;
class event_loop
{
public:
	event_loop();
	~event_loop();
	event_loop(const event_loop &loop) = delete;

	//�¼�ѭ��
	void loop();
	//ע���¼�
	bool register_event();

private:
	std::map<int, std::shared_ptr<session>>  _handler;  //<<�׽��֣��¼����>>
};

