#pragma once

#include <memory>

#include <QObject>
#include <QAbstractSocket>

#include <network/commands/abstractcommand.h>

#include "networkendpoint.h"

namespace terminus
{
	class NetworkConnection;

    class NetworkClient : public NetworkEndpoint
	{
		Q_OBJECT
	public:
        NetworkClient(QObject *parent = 0);
		virtual ~NetworkClient();

		void connectToHost(QString host, unsigned short port);
		void disconnect();

		QString host();
        quint16 port();

	public slots:
		void setHost(QString host);
		void setPort(QString port);
        void setPort(quint16 port);
		void socketConnected();
        void socketDisconnected();
	private:
		void connectToDefaultHost();
        void sendMessage(QJsonDocument &jsonDocument);

		QString m_host;
		quint16 m_port;
        NetworkConnection *m_connection;
	};
}
