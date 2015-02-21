#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "networkserver.h"
#include "networkclient.h"
#include "networkconnection.h"

namespace PCLIB
{
    NetworkClient::NetworkClient(NetworkConnection *client, QObject *parent)
		: QObject(parent)
		, m_useRemotePrefixForStatus(useRemotePrefixForStatus)
		, m_serverStatus("")
		, m_percentComplete(0)
	{
		m_client = client != nullptr ? client : new NetworkConnection(this);

		m_host = "localhost";
		m_port = 4711;

		connect(m_client, &NetworkConnection::connected, this, &NetworkClient::socketConnected);
		connect(m_client, &NetworkConnection::disconnected, this, &NetworkClient::socketDisconnected);
		connect(m_client, SIGNAL(readyRead()), this, SLOT(readMessage()));
		connect(m_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
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
		m_client->connectToHost(host, port);
	}

	void NetworkClient::connectToDefaultHost() {
		connectToHost(m_host, m_port);
	}

	void NetworkClient::disconnect() {
		m_client->abort();
		m_client->close();
	}

	void NetworkClient::readMessage() {
		quint16 blockSize = 0;

		auto in = m_client->inDataStream();

		while (m_client->bytesAvailable() > (int)sizeof(quint16)) {
			*in >> blockSize;

			if (m_client->bytesAvailable() < blockSize)
				return;

			QString str;
			*in >> str;
			//qDebug() << "server response: " << str;

			QJsonDocument jsonDocument = QJsonDocument::fromJson(str.toUtf8());
			switch (jsonDocument.object().value("messageType").toInt()) {
			case ServerMessages::Progress:
				qDebug() << "Progress: " << jsonDocument.object().value("progress").toObject().value("percentComplete").toInt();
				updateProgress(jsonDocument);
				break;
			case ServerMessages::Result:
				emit operationOngoing(false);

				handleResult(jsonDocument.object()["result"].toObject());
				break;
			case ServerMessages::Log:
				showLogMessage(jsonDocument.object()["logMsg"].toString());
				break;
			default:
				qDebug() << "I don't know what kind of server reply this is!";
				break;
			}
		}
	}

	void NetworkClient::socketConnected()
	{
		qDebug() << "Socket to " << host() << ":" << port() << " connected!";
		if (!m_dataToBeSent.isEmpty())
		{
			QByteArray debugByteArray(m_dataToBeSent);
			QDataStream debugInStream(debugByteArray);
			debugInStream.setVersion(QDataStream::Qt_5_2);

			quint16 debugMsgSize;
			QString debugMsgString;

			debugInStream >> debugMsgSize >> debugMsgString;
			qDebug() << "sending data that was held back for " << host() << ":" << port() << "('" << debugMsgString << "', " << debugMsgSize << " bytes)";

			m_client->write(m_dataToBeSent);
			m_dataToBeSent.clear();
		}
	}

    void NetworkClient::socketDisconnected()
    {
        qDebug() << "Socket to " << host() << ":" << port() << " disconnected!";
    }

	void NetworkClient::updateProgress(QJsonDocument &jsonDocument) {
		QString remoteStatus = jsonDocument.object()["progress"].toObject()["status"].toString();
		if (m_useRemotePrefixForStatus)
			m_serverStatus = "[Remote] " + remoteStatus;
		else
			m_serverStatus = remoteStatus;

		m_percentComplete = jsonDocument.object()["progress"].toObject()["percentComplete"].toInt();
		
		emit updateProgressValue(m_percentComplete);
		emit updateProgressLabel(m_serverStatus);
	}

	void NetworkClient::handleResult(QJsonObject result) {

		if (result.value("result").toString() == NetworkServer::serverBusyMessage()) {
			QMessageBox msgBox;
			msgBox.setText(NetworkServer::serverBusyMessage());
			msgBox.exec();
		}

		m_commandResult = result;

		switch (m_currentCommand) {
		case Command_Hello:
			qDebug() << "Result: " << result.value("result").toString();
			break;
		case Command_TopologicalAnalysis:
			qDebug() << "Result: " << result.value("result").toString();
			break;
		case Command_DifferenceAnalysis:
			qDebug() << "Result: " << result.value("result").toString();
			break;
		case Command_BuildingAnalysis:
			qDebug() << "Result: " << result.value("result").toString();
			break;
		case Command_ListDirectory:
			emit listRemoteFilesCompleted(result);
			break;
		default:
			qDebug() << "Result: " << result.value("result").toString();
		}
	}

	void NetworkClient::displayError(QAbstractSocket::SocketError error) {
		QString errorMsg;
		QDebug(&errorMsg) << error;
		if (error == QAbstractSocket::RemoteHostClosedError)
		{
			showLogMessage(errorMsg);
			operationOngoing(false);
			disconnect();
		}
	}

	void NetworkClient::sendHelloRequest() {
		invokeRemoteCommand(
			Command_Hello,
			nullptr,
			QString("[Remote] Starting Test Command..."));
	}

	void NetworkClient::startTopologicalAnalysis(std::shared_ptr<TopoAnalysisSettings> settings) {
		invokeRemoteCommand(
			Command_TopologicalAnalysis,
			settings,
			QString("[Remote] Starting Topological Analysis..."));
	}

	void NetworkClient::startBuildingAnalysis(std::shared_ptr<BuildingAnalysisSettings> settings) {
		invokeRemoteCommand(
			Command_BuildingAnalysis,
			settings,
			QString("[Remote] Starting Building Analysis..."));
	}

	void NetworkClient::startDiffAnalysis(std::shared_ptr<DiffAnalysisSettings> settings) {
		invokeRemoteCommand(
			Command_DifferenceAnalysis,
			settings,
			QString("[Remote] Starting Difference Analysis..."));
	}

	void NetworkClient::notifyAgent(QString myHost, unsigned short myPort) {
		m_currentCommand = Command_Notification;

		QJsonObject paramObject;
		paramObject.insert("pctoolHost", myHost);
		paramObject.insert("pctoolPort", myPort);

		QJsonObject jsonObject;
		jsonObject.insert("commandType", m_currentCommand);
		jsonObject.insert("parameter", paramObject);

		QJsonDocument jsonDocument;
		jsonDocument.setObject(jsonObject);

		sendMessage(jsonDocument);

		emit operationOngoing(true);
		emit updateProgressLabel(m_serverStatus = "Notifying agent");
		emit updateProgressValue(m_percentComplete = 0);
	}

	void NetworkClient::startRemoteExecution(Commands commandType, QJsonObject params) {
		m_currentCommand = commandType;

		QJsonObject jsonObject;
		jsonObject.insert("commandType", m_currentCommand);
		jsonObject.insert("parameter", params);

		QJsonDocument jsonDocument;
		jsonDocument.setObject(jsonObject);

		sendMessage(jsonDocument);

		emit operationOngoing(true);
		emit updateProgressLabel(m_serverStatus = "Waiting for PCTool...");
		emit updateProgressValue(m_percentComplete = 0);
	}

	void NetworkClient::listRemoteFiles(const QString currentDirectory, const QString changeDirectory, const QStringList fileFilter) {
		m_currentCommand = Command_ListDirectory;
		
		QJsonArray fileFilterParam;
		for (auto f : fileFilter) {
			fileFilterParam.append(f);
		}
		
		QJsonObject paramObject;
		paramObject.insert("currentDirectory", currentDirectory);
		paramObject.insert("changeDirectory", changeDirectory);
		paramObject.insert("fileFilter", fileFilterParam);

		QJsonObject jsonObject;
		jsonObject.insert("commandType", m_currentCommand);
		jsonObject.insert("parameter", paramObject);

		QJsonDocument jsonDocument;
		jsonDocument.setObject(jsonObject);

		sendMessage(jsonDocument);

		emit operationOngoing(true);
		emit updateProgressLabel(m_serverStatus = "Retrieving file list");
		emit updateProgressValue(m_percentComplete = 0);
	}

	void NetworkClient::invokeRemoteCommand(Commands commandType, std::shared_ptr<AnalysisSettingsBase> settings, QString initialProgressLabel) {
		m_currentCommand = commandType;

		if (settings != nullptr)
			m_singleFileModeFileName = settings->sourceFileName();		
		else 
			m_singleFileModeFileName = "";
		
		QJsonObject jsonObject;
		jsonObject.insert("commandType", m_currentCommand);
		
		if (settings != nullptr)
			jsonObject.insert("parameter", settings->getJson());

		QJsonDocument jsonDocument;
		jsonDocument.setObject(jsonObject);

		sendMessage(jsonDocument);

		emit operationOngoing(true);
		emit updateProgressLabel(m_serverStatus = initialProgressLabel);
		emit updateProgressValue(m_percentComplete = 0);
	}

	void NetworkClient::sendMessage(QJsonDocument &jsonDocument) {
		connectToDefaultHost();
		qDebug() << "Sending JSON:   " << QString(jsonDocument.toJson(QJsonDocument::JsonFormat::Compact));

		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_5_2);

		// message size will go here
		out << (quint16) 0;

		out << QString(jsonDocument.toJson(QJsonDocument::JsonFormat::Compact));

		// write message size
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));


		// check if socket is ready
		if (m_client->isConnected())
		{
			m_client->write(block);
		}
		else
		{
			// schedule data to be sent when socket is connected
			if (m_dataToBeSent.isEmpty())
			{
				QByteArray debugByteArray(block);
				QDataStream debugInStream(debugByteArray);
				debugInStream.setVersion(QDataStream::Qt_5_2);

				quint16 debugMsgSize;
				QString debugMsgString;

				debugInStream >> debugMsgSize >> debugMsgString;

				qDebug() << "... holding back data for " << host() << ":" << port() << " ('" << debugMsgString << "', " << debugMsgSize << " bytes)";
				m_dataToBeSent = block;
			}
			else
			{
				QByteArray debugByteArray(block);
				QDataStream debugInStream(debugByteArray);
				debugInStream.setVersion(QDataStream::Qt_5_2);

				quint16 debugMsgSize;
				QString debugMsgString;

				debugInStream >> debugMsgSize >> debugMsgString;
				qWarning() << "there is already data waiting to be sent to " << host() << ":" << port() << ", discarding this packet! ('" << debugMsgString << "', " << debugMsgSize << " bytes)";
			}
		}
	}

	void NetworkClient::showLogMessage(QString msg) {
		qDebug() << msg;
		emit newLogMessage(msg);
	}

	Commands NetworkClient::currentCommand() {
		return m_currentCommand;
	}

	QString NetworkClient::singleFileModeFileName() {
		return m_singleFileModeFileName;
	}

	QString NetworkClient::serverStatus() {
		return m_serverStatus;
	}

	int NetworkClient::percentComplete() {
		return m_percentComplete;
	}

	QString NetworkClient::host() {
		return m_host;
	}

	quint16 NetworkClient::port() {
		return m_port;
	}

	QJsonObject NetworkClient::commandResult() {
		return m_commandResult;
	}
}
