#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "networkserver.h"
#include "networkclient.h"
#include "networkconnection.h"

namespace terminus
{
    NetworkClient::NetworkClient(QObject *parent)
        : NetworkEndpoint(parent)
        , m_connection(new NetworkConnection(this))
        , m_host("localhost")
        , m_port(4711)
	{
        connect(m_connection, &NetworkConnection::connected, this, &NetworkClient::socketConnected);
        connect(m_connection, &NetworkConnection::disconnected, this, &NetworkClient::socketDisconnected);
        connect(m_connection, &NetworkConnection::readyRead, this, &NetworkClient::receiveMessages);
	}

	NetworkClient::~NetworkClient() {
		disconnect();
	}

	void NetworkClient::setHost(QString host) {
		m_host = host;
	}

	void NetworkClient::setPort(QString port) {
		m_port = port.toInt();
	}

	void NetworkClient::setPort(quint16 port) {
		m_port = port;
	}

	void NetworkClient::connectToHost(QString host, unsigned short port) {
		qDebug() << "Connecting to " << host << ":" << port;
        m_connection->connectToHost(host, port);
	}

	void NetworkClient::connectToDefaultHost() {
		connectToHost(m_host, m_port);
	}

	void NetworkClient::disconnect() {
        m_connection->abort();
        m_connection->close();
    }

	void NetworkClient::socketConnected()
	{
        m_activePlayerConnection = m_connection;
	}

    void NetworkClient::socketDisconnected()
    {
        m_activePlayerConnection = nullptr;
        qDebug() << "Socket to " << host() << ":" << port() << " disconnected!";
    }

    void NetworkClient::sendMessage(QJsonDocument &jsonDocument) {
        NetworkEndpoint::sendMessage(m_connection, jsonDocument);
	}

	QString NetworkClient::host() {
		return m_host;
	}

	quint16 NetworkClient::port() {
		return m_port;
	}
}
