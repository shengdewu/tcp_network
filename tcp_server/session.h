#pragma once
class session
{
public:
	session();
	virtual ~session();

	//��д�¼�����
	void notify_read_event();
	void notify_write_event();

protected:
	//��д����
	virtual bool handle_read_event() = 0;
	virtual bool handle_write_event() = 0;

private:
	//ע���д�¼�
	void post_read_event();
	void post_write_event();

private:
	int	_fd;   //�׽���
};

