#pragma once

#include <QObject>
#include <QTimer>

#include <memory>
#include <functional>

#include <network/commands/abstractcommand.h>
#include "networkserverbase.h"

namespace terminus
{
	class TcpServer;

	class NetworkServer : public NetworkServerBase
	{
		Q_OBJECT
	public:
		NetworkServer(TcpServer* server = nullptr, QObject* parent = 0);
        ~NetworkServer();
	protected:
		virtual AbstractCommand* createCommandForRequest(NetworkConnection* clientConnection, const QString &request) override;
		void handleNewCommand(AbstractCommand* command) override;
		void deleteCommand(AbstractCommand* command) override;
	};
}
