#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <functional>
#include <memory>

namespace PCLIB
{
	class NetworkConnection;

	class TcpServer : public QObject
	{
		Q_OBJECT
	public:
		TcpServer(QObject *parent = 0, std::function<NetworkConnection*(QTcpSocket*)> networkConnectionFactory = nullptr);

		virtual bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);
		virtual NetworkConnection* nextPendingConnection();

	signals:
		void newConnection();

	private:
		QTcpServer* m_server;
		std::function<NetworkConnection*(QTcpSocket*)> m_networkConnectionFactory;

		void onServerNewConnection();
	};
}