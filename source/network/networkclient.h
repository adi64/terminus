#pragma once

#include <memory>

#include <QObject>
#include <QAbstractSocket>

#include <network/commands/abstractcommand.h>

#include "networkendpoint.h"

namespace terminus
{
	class NetworkConnection;

    class NetworkClient : public NetworkEndpoint, public QObject
	{
		Q_OBJECT
	public:
        NetworkClient(NetworkConnection *connection = nullptr, QObject *parent = 0);
		virtual ~NetworkClient();

		void connectToHost(QString host, unsigned short port);
		void disconnect();

		void notifyAgent(QString host, unsigned short port);

		QString host();
        quint16 port();

	public slots:
		void setHost(QString host);
		void setPort(QString port);
		void setPort(quint16 port);
		void readMessage();
		void displayError(QAbstractSocket::SocketError error);
		void socketConnected();
        void socketDisconnected();
	private:
		void connectToDefaultHost();
		void sendMessage(QJsonDocument &jsonDocument);
		void handleResult(QJsonObject result);

		QString m_host;
		quint16 m_port;
        NetworkConnection *m_connection;

		QByteArray m_dataToBeSent;
	};
}
