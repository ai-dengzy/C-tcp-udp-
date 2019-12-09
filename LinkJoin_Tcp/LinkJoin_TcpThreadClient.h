#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QFutureWatcher>
#include <QQueue>
#include <QThread>

#include "LinkJoin.h"

class LJ_TcpThreadClient : public QTcpSocket {
	Q_OBJECT
public:
	explicit LJ_TcpThreadClient(const qintptr &socket_descriptor, QObject *parent = Q_NULLPTR);
	~LJ_TcpThreadClient();

	QByteArray HandleData(const int &socket_id, const QString &QString, const quint16 &port, QByteArray data);	// �����������ݵĺ���

protected:
	QFutureWatcher<QByteArray> __watcher;
	QQueue<QByteArray> __datas;

private:
	qintptr _socket_id; //  �׽���

signals:
	void Signal_SendToServer(const int &socket_id, const QString &QString, const quint16 &port, const QByteArray &data);	// client����������
	void Signal_DisconnectedWithServer(const int &socket_id, const QString &QString, const quint16 &port, QThread *thread_id);	// ���ͶϿ����ӵ��û���Ϣ ���źű��뷢��! �̹߳���������źż�����

public slots:
	void Slot_SendToClient(const int &socket_id, const QByteArray &data);	// ��������
	void Slot_DisconnectedWithClient(const int &socket_id);	// �Ͽ����� -1ȫ���Ͽ�

protected slots:
	void Slot_Receive();	// ��ȡ����
	void Slot_StartNext();	// ������һ��
};
