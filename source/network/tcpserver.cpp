#include "tcpserver.h"
#include "networkconnection.h"

namespace PCLIB
{
	TcpServer::TcpServer(QObject* parent, std::function<NetworkConnection*(QTcpSocket*)> networkConnectionFactory)
		: QObject(parent)
        , m_server(new QTcpServer(this))
		, m_networkConnectionFactory(networkConnectionFactory)
    {
		connect(m_server, &QTcpServer::newConnection, this, &TcpServer::newConnection);
	}

	bool TcpServer::listen(const QHostAddress &address, quint16 port) {
		return m_server->listen(address, port);
	}

	NetworkConnection* TcpServer::nextPendingConnection() {
		auto socket = m_server->nextPendingConnection();
		connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

		NetworkConnection* conn;
		if (m_networkConnectionFactory != nullptr) {
			conn = m_networkConnectionFactory(socket);
		}
		else {
			conn = NetworkConnection::fromTcpSocket(socket);
		}
		return conn;
	}
}
