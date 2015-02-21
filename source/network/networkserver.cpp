#include "networkserver.h"

#include <QDebug>
#include <QHostAddress>

#include <network/commands/abstractcommand.h>
#include "networkconnection.h"

namespace terminus
{
    NetworkServer::NetworkServer(QObject* parent)
        : NetworkEndpoint(parent)
        , m_server(new QTcpServer(this))
		, m_listenPort(4711)
		, m_listenPortForced(false)
	{
        connect(m_server, &QTcpServer::newConnection, this, &NetworkServer::newConnection);
	}

    QString NetworkServer::serverBusyMessage()
	{
		return "Command denied: Server is busy";
	}

    bool NetworkServer::start() {
		unsigned short originalListenPort = m_listenPort;

        while (!m_server->listen(QHostAddress::Any, m_listenPort)) {
			m_listenPort++;

			// error on overflow
			if (m_listenPortForced ||  m_listenPort < originalListenPort) {
				qDebug() << "Could not start server";
				return false;
			}
		}

		qDebug() << "Listening on Port" << m_listenPort;

		emit listening();

		return true;
	}

    void NetworkServer::setListenPort(unsigned short port)
	{
		if (port != 0)
		{
			m_listenPort = port;
			m_listenPortForced = true;
		}
		else
		{
			m_listenPort = 4711;
			m_listenPortForced = false;
		}
	}

    unsigned short NetworkServer::listenPort() {
		return m_listenPort;
	}

    void NetworkServer::newConnection() {
        auto socket = m_server->nextPendingConnection();
        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

        auto clientConnection = NetworkConnection::fromTcpSocket(socket);

        connect(clientConnection, &NetworkConnection::disconnected, this, &NetworkServer::clientDisconnected);
        connect(clientConnection, &NetworkConnection::readyRead, this, &NetworkServer::receiveMessages);
	}

    void NetworkServer::denyCommand(AbstractCommand* command) {
		QJsonObject result;
		result.insert("result", serverBusyMessage());

		QJsonObject jsonObject;
		jsonObject.insert("messageType", ServerMessages::Result);
		jsonObject.insert("result", result);

		QJsonDocument jsonDocument;
		jsonDocument.setObject(jsonObject);

		sendMessage(command->clientConnection(), jsonDocument);
		command->clientConnection()->disconnectFromHost();
	}

    void NetworkServer::clientDisconnected() {
		auto connection = dynamic_cast<NetworkConnection*>(sender());
		if (!connection)
		{
			qDebug() << "Sender was no NetworkConnection";
			return;
		}

		auto command = dynamic_cast<AbstractCommand*>(connection->parent());
		if (!command)
		{
			qDebug() << "Parent of NetworkConnection was no AbstractCommand";
			return;
		}
	}
}
