#pragma once

#include <QObject>

#include <memory>

#include "networkendpoint.h"

namespace terminus
{
	class AbstractCommand;
	class NetworkConnection;
    class QTcpServer;

    class NetworkServer : public QObject
	{
		Q_OBJECT
	public:
        NetworkServer(TcpServer* server = nullptr, QObject* parent = 0);

		static QString serverBusyMessage();
		void setListenPort(unsigned short port);
        unsigned short listenPort();

	public slots:
		bool start();
	signals:
		void listening();
        void shutdown();
	protected:
		void denyCommand(AbstractCommand* command);
	protected slots:
		void clientDisconnected();
	private:
        QTcpServer* m_server;
		unsigned short m_listenPort;
		bool m_listenPortForced;
		quint16 m_expectedMessageSize;
	private slots:
		void newConnection();
	};
}
