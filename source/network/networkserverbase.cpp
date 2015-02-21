#include "networkserverbase.h"

#include <QDebug>
#include <iostream>
#include <QHostAddress>
#include <QSharedPointer>

#include <network/commands/abstractcommand.h>
#include "networkconnection.h"
#include "tcpserver.h"

namespace PCLIB
{
	NetworkServerBase::NetworkServerBase(TcpServer* server, QObject* parent)
		: QObject(parent)
		, m_listenPort(4711)
		, m_listenPortForced(false)
		, m_expectedMessageSize(0)
	{
		m_tcpServer = server == nullptr ? new TcpServer(this) : server;
		connect(m_tcpServer, &TcpServer::newConnection, this, &NetworkServerBase::newConnection);
	}

	QString NetworkServerBase::serverBusyMessage()
	{
		return "Command denied: Server is busy";
	}

	int NetworkServerBase::returnCodeNetworkServerFail()
	{
		return 10;
	}

	bool NetworkServerBase::start() {
		unsigned short originalListenPort = m_listenPort;

		while (!m_tcpServer->listen(QHostAddress::Any, m_listenPort)) {
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

	void NetworkServerBase::setListenPort(unsigned short port)
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

	unsigned short NetworkServerBase::listenPort() {
		return m_listenPort;
	}

	void NetworkServerBase::newConnection() {
		auto clientConnection = m_tcpServer->nextPendingConnection();
		connect(clientConnection, &NetworkConnection::disconnected, this, &NetworkServerBase::clientDisconnected);
		connect(clientConnection, &NetworkConnection::readyRead, this, &NetworkServerBase::readFromClient);
	}

	void NetworkServerBase::readFromClient() {
		NetworkConnection* socket = dynamic_cast<NetworkConnection*>(sender());
		
		auto in = socket->inDataStream();

		// are we receiving the beginning of a message?
		if (m_expectedMessageSize == 0) {
			quint16 blockSize = 0;	

			if (socket->bytesAvailable() < (int)sizeof(quint16))
				return;

			*in >> blockSize;

			m_expectedMessageSize = blockSize;
		}

		// wait until the rest of the message is received, if necessary
		if (socket->bytesAvailable() < m_expectedMessageSize)
			return;

		QString str;
		*in >> str;

		m_expectedMessageSize = 0;

		qDebug() << "client message: " << str;

		AbstractCommand* cmd = createCommandForRequest(socket, str);
		socket->setParent(cmd); // delete connection object when command is completed
		
		connect(cmd, &AbstractCommand::done, this, &NetworkServerBase::completeCommand);

		handleNewCommand(cmd);
	}

	void NetworkServerBase::sendMessage(NetworkConnection* client, QJsonDocument &message) {
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_5_2);

		QString msgString = QString(message.toJson(QJsonDocument::JsonFormat::Compact));

		// message size will go here
		out << (quint16) 0;

		out << msgString;

		// write message size
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));

		client->write(block);
	}

	void NetworkServerBase::completeCommand() {
		AbstractCommand* cmd = dynamic_cast<AbstractCommand*>(sender());
		if (cmd == nullptr) {
			qDebug() << "Sender was no AbstractCommand";
			return;
		}

		QJsonObject result = cmd->getResultsAsJsonObject();

		QJsonObject jsonObject;
		jsonObject.insert("messageType", ServerMessages::Result);
		jsonObject.insert("result", result);

		QJsonDocument jsonDocument;
		jsonDocument.setObject(jsonObject);
		sendMessage(cmd->clientConnection(), jsonDocument);
		cmd->clientConnection()->disconnectFromHost();
	}

	void NetworkServerBase::denyCommand(AbstractCommand* command) {
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

	void NetworkServerBase::clientDisconnected() {
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

		bool exit = command->exitOnComplete();

		deleteCommand(command);

		emit commandCompleted(exit);
	}

	void NetworkServerBase::sendLogMessageToClient(const QString &msg, NetworkConnection* client) {
		QJsonObject jsonObject;
		jsonObject.insert("messageType", ServerMessages::Log);
		jsonObject.insert("logMsg", msg);

		QJsonDocument jsonDocument;
		jsonDocument.setObject(jsonObject);

		sendMessage(client, jsonDocument);
	}

	void NetworkServerBase::reportProgressToClient(int percentComplete, const QString &status, NetworkConnection* client) {
		QJsonObject statusObject;
		statusObject.insert("percentComplete", percentComplete);
		statusObject.insert("status", status);

		QJsonObject jsonObject;
		jsonObject.insert("messageType", ServerMessages::Progress);
		jsonObject.insert("progress", statusObject);

		QJsonDocument jsonDocument;
		jsonDocument.setObject(jsonObject);

		sendMessage(client, jsonDocument);
	}
}
