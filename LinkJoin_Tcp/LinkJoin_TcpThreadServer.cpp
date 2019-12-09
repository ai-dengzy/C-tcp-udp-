#include <QTcpSocket>

#include "LinkJoin_TcpThreadServer.h"
#include "LinkJoin.h"
#include "LinkJoin_TcpThreadHandle.h"

LJ_TcpThreadServer::LJ_TcpThreadServer(QObject *parent, const int &max_connections)
	: QTcpServer(parent) {
	_tcp_client_map = new QHash<int, LJ_TcpThreadClient *>;
	setMaxPendingConnections(max_connections);
}

LJ_TcpThreadServer::~LJ_TcpThreadServer() {
	emit this->Signal_DisconnectWithClient(-1);
	delete _tcp_client_map;
}

void LJ_TcpThreadServer::setMaxPendingConnections(const int & max_connections) {
	this->QTcpServer::setMaxPendingConnections(max_connections);	// ����Qtcpsocket���� ������������� ��Ҫ��ʹmaxPendingConnections()��Ȼ��Ч
	this->_max_connections = max_connections;
}

void LJ_TcpThreadServer::close() {
	emit this->Signal_DisconnectWithClient(-1);
	_tcp_client_map->clear();
	QTcpServer::close();
}

void LJ_TcpThreadServer::incomingConnection(qintptr socket_id) {
	if (_tcp_client_map->size() > maxPendingConnections()) {	// �̳���д�˺����� QTcpServerĬ�ϵ��ж����������ʧЧ �Լ�ʵ��
		QTcpSocket tcp;
		tcp.setSocketDescriptor(socket_id);
		tcp.disconnectFromHost();
		return;
	}
	auto th = LJ_TcpThreadHandle::GetInstance().GetThread();
	auto client = new LJ_TcpThreadClient(socket_id);

	connect(client, &LJ_TcpThreadClient::Signal_DisconnectedWithServer, this, &LJ_TcpThreadServer::Slot_ClientDisconnected);	// �Ͽ����ӵĴ��� ���б��Ƴ� ���ͷŶϿ���Tcpsocket �˲۱���ʵ�� �̹߳������Ҳ�ǿ�����
	connect(client, &LJ_TcpThreadClient::Signal_SendToServer, this, &LJ_TcpThreadServer::Slot_ClientReceive);	// �Ͽ����ӵĴ��� ���б��Ƴ� ���ͷŶϿ���Tcpsocket �˲۱���ʵ�� �̹߳������Ҳ�ǿ�����
	connect(this, &LJ_TcpThreadServer::Signal_DisconnectWithClient, client, &LJ_TcpThreadClient::Slot_DisconnectedWithClient);	// ���� �Ͽ� �� �ź�
	connect(this, &LJ_TcpThreadServer::Signal_SendToClient, client, &LJ_TcpThreadClient::Slot_SendToClient);	// ���͸�client

	client->moveToThread(th);	// ��tcp���ƶ����µ��߳� ���̹߳������л�ȡ
	_tcp_client_map->insert(socket_id, client);	// ���뵽������Ϣ��
	QString ip = client->peerAddress().toString().remove("::ffff:");
	int port = client->peerPort();
	emit Signal_ClientConnected(socket_id, ip, port);
	//LJ_DEBUG(tr("�߳�0x%1�ͻ���[%2]%3:%4 ������").arg(quintptr(th), 8, 16, QLatin1Char('0')).arg(socket_id).arg(ip).arg(port));
}

void LJ_TcpThreadServer::Slot_SendToClient(const int &socket_id, const QByteArray &data) {	// �����ͻ�������
	emit Signal_SendToClient(socket_id, data);
}

void LJ_TcpThreadServer::Slot_ClientReceive(const int &socket_id, const QString &ip, const int &port, const QByteArray &data) {
	emit Signal_ClientReceive(socket_id, ip, port, data);
}

void LJ_TcpThreadServer::Slot_DisconnectWithClient(const int & socket_id) {
	emit Signal_DisconnectWithClient(socket_id);
}

void LJ_TcpThreadServer::Slot_ClientDisconnected(const int &socket_id, const QString &ip, const quint16 &port, QThread *th) {
	_tcp_client_map->remove(socket_id);	// ���ӹ������Ƴ��Ͽ����ӵ�socket
	//LJ_DEBUG(tr("�߳�0x%1�ͻ���[%2]%3:%4 �ѶϿ�").arg(quintptr(th), 8, QLatin1Char('0')).arg(socket_id).arg(ip).arg(port));
	LJ_TcpThreadHandle::GetInstance().ReduceThread(th);	// �����̹߳������Ǹ��߳�������ӶϿ���
	emit Signal_ClientDisconnected(socket_id, ip, port);	// �����Ͽ����ӵ�client��Ϣ
}