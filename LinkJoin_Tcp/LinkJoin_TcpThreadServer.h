#pragma once

#include <QObject>
#include <QTcpServer>
#include <QThread>

#include "LinkJoin_TcpThreadClient.h"

/*
	TCP���̷߳����� 1.0.0
	vanxkr.com vanxkr@qq.com ������
	2019-12-9 17:20:59
*/
class LJ_TcpThreadServer : public QTcpServer {
	Q_OBJECT
public:
	explicit LJ_TcpThreadServer(QObject *parent = Q_NULLPTR, const int &max_connections = 100);
	~LJ_TcpThreadServer();

	void setMaxPendingConnections(const int &max_connections);	// ��д�����������
	void close();	// �رշ����� ͬʱҲ��Ͽ���������

protected:
	void incomingConnection(qintptr socket_id) override;	// ֻҪ����һ���µ����ӣ��ͻ��Զ������������

private:
	int _max_connections;	// ���������
	QHash<int, LJ_TcpThreadClient *> *_tcp_client_map;	// �ͻ������ӹ���

signals:
	void Signal_ClientConnected(const int &socket_id, const QString &ip, const quint16 &port);	// �ͻ�������
	void Signal_ClientReceive(const int &socket_id, const QString &ip, const int &port, const QByteArray &data);	// �ͻ��˷���������
	void Signal_ClientDisconnected(const int &socket_id, const QString &ip, const int &port);	// �ͻ��˶Ͽ�

	void Signal_SendToClient(const int &socket_id, const QByteArray &data);	// �������ݸ��ͻ���
	void Signal_DisconnectWithClient(const int &socked_id);	// ���� �Ͽ� �ͻ��� -1Ϊ ȫ���Ͽ�

public slots:
	void Slot_SendToClient(const int &socket_id, const QByteArray &data);	// �����ͻ�������
	void Slot_DisconnectWithClient(const int &socket_id);	// �Ͽ���ͻ��˵�����

protected slots:
	void Slot_ClientReceive(const int &socket_id, const QString &ip, const int &port, const QByteArray &data);	// �ͻ��˷�������
	void Slot_ClientDisconnected(const int &socket_id, const QString &ip, const quint16 &port, QThread *th);	// �ͻ��˶Ͽ�����

};
