#pragma once

#include <memory>

#include <QObject>
#include <QTcpServer>

#include "networkendpoint.h"

namespace terminus
{
	class AbstractCommand;
	class NetworkConnection;

    class NetworkServer : public NetworkEndpoint
	{
		Q_OBJECT
	public:
        NetworkServer(QObject* parent = 0);

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
	private slots:
		void newConnection();
	};
}
