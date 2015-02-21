#include "networkserver.h"

#include <QDebug>

#include <network/commands/listdirectorycommand.h>

namespace PCLIB
{
	NetworkServer::NetworkServer(TcpServer* server, QObject* parent)
		: NetworkServerBase(server, parent)
		, m_runningCommand(nullptr)
	{
		connect(&m_progressTimer, &QTimer::timeout, this, &NetworkServer::reportProgress);
	}

	NetworkServer::~NetworkServer() {
		if (m_runningCommand != nullptr)
			delete m_runningCommand;
	}

	void NetworkServer::setClassificationInterface(ClassificationInterface* clInt)
	{
		m_clInt = clInt;
	}

	void NetworkServer::setDistanceAnalysisInterfaceFactory(std::function<IDistanceAnalysisInterface*()> compareIntFactory) {
		m_compareIntFactory = compareIntFactory;
	}

	void NetworkServer::handleNewCommand(AbstractCommand* command)
	{
		if (m_runningCommand) {
			denyCommand(command);
			delete command;
			qDebug() << "client requests new operation but there already is a command running!";
			return;
		}

		m_runningCommand = command;

		m_progressTimer.setInterval(command->defaultProgressInterval());
		m_progressTimer.start();
		command->run();
	}

	void NetworkServer::sendLogMessage(const QString &msg) {
		if (m_runningCommand == nullptr)
			return;

		sendLogMessageToClient(msg, m_runningCommand->clientConnection());
	}
	
	void NetworkServer::reportProgress() {
		if (m_runningCommand == nullptr)
			return;

		reportProgressToClient(m_runningCommand->percentComplete(), 
							   m_runningCommand->status(), 
							   m_runningCommand->clientConnection());
	}

	AbstractCommand* NetworkServer::createCommandForRequest(NetworkConnection* clientConnection, const QString &request) {
		QJsonDocument json = QJsonDocument::fromJson(request.toUtf8());
		AbstractCommand* cmd;

		int type = json.object()["commandType"].toInt();
		switch (type) {
		case Commands::Command_Hello:
			cmd = new TestCommand(clientConnection, m_clInt);
			break;
		case Commands::Command_TopologicalAnalysis:
			cmd = new TopoAnalysisCommand(clientConnection, m_clInt);
			break;
		case Commands::Command_BuildingAnalysis:
			cmd = new BuildingAnalysisCommand(clientConnection, m_clInt);
			break;
		case Commands::Command_DifferenceAnalysis:
			cmd = new DiffAnalysisCommand(clientConnection, m_clInt, m_compareIntFactory);
			break;
		case Command_ListDirectory:
			cmd = new ListDirectoryCommand(clientConnection, nullptr);
			break;
			// ...

		default:
			// todo: we should define an error response...
			qDebug() << "error parsing client request";
			cmd = new TestCommand(clientConnection, m_clInt);
		}

		cmd->initializeFromJson(json.object()["parameter"].toObject());
		return cmd;
	}

	void NetworkServer::deleteCommand(AbstractCommand* command) {
		m_progressTimer.stop();

		if (command != m_runningCommand) {
			qDebug() << "Completed command was not currently active command";
			return;
		}

		delete m_runningCommand;
		m_runningCommand = nullptr;
	}
}
