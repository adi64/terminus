#pragma once

#include <QObject>

#include <memory>

namespace PCLIB
{
	class AbstractCommand;
	class NetworkConnection;
	class TcpServer;

	class NetworkServerBase : public QObject
	{
		Q_OBJECT
	public:
		NetworkServerBase(TcpServer* server = nullptr, QObject* parent = 0);

		static QString serverBusyMessage();
		void setListenPort(unsigned short port);
		unsigned short listenPort();
		static int returnCodeNetworkServerFail();

	public slots:
		bool start();
	signals:
		void listening();
		void shutdown();
		void commandCompleted(bool exit);
	protected:
		virtual AbstractCommand* createCommandForRequest(NetworkConnection* clientConnection, const QString &request) = 0;
		virtual void handleNewCommand(AbstractCommand* command) = 0;
		virtual void deleteCommand(AbstractCommand* command) = 0;
		void sendMessage(NetworkConnection* client, QJsonDocument &message);
		void denyCommand(AbstractCommand* command);
	protected slots:
		void clientDisconnected();
	private:
		TcpServer* m_tcpServer;
		unsigned short m_listenPort;
		bool m_listenPortForced;
		quint16 m_expectedMessageSize;
	private slots:
		void newConnection();
		void readFromClient();
		void completeCommand();
	};
}
