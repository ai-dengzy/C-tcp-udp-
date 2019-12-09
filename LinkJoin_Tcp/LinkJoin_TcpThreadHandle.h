#pragma once

#include <QThread>
#include <QHash>

// �̹߳����� ������һ���̳߳� ������
// ���ֳ�ʼ����ʽ һ����ÿ���̴߳���������� һ����һ�����ٸ��߳�
class LJ_TcpThreadHandle {
public:
	~LJ_TcpThreadHandle();

	static LJ_TcpThreadHandle & GetInstance();	// ����

	QThread *GetThread();	// ��ȡһ���������ӵ��߳�
	void	ReduceThread(QThread *thread);	// ����һ���߳�����

private:
	LJ_TcpThreadHandle();
	const unsigned					_kThreadMax = 10;	// ÿ���̴߳����������
	QHash<QThread *, unsigned int>	_thread_list;	// �߳��б�
};

