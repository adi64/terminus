#include "networkendpoint.h"

#include <QJsonDocument>

#include <network/commands/abstractcommand.h>
#include <network/commands/clientreadycommand.h>
#include <network/commands/pausecommand.h>
#include <network/commands/projectilefiredcommand.h>
#include <network/commands/projectilehitcommand.h>
#include <network/commands/preparenewgamecommand.h>
#include <network/networkconnection.h>

namespace terminus
{

NetworkEndpoint::NetworkEndpoint(QObject *parent)
    : QObject(parent)
    , m_expectedMessageSize(0)
    , m_activePlayerConnection(nullptr)
{

}

void NetworkEndpoint::sendMessage(AbstractCommand *command)
{
    if(m_activePlayerConnection)
    {
        sendMessage(m_activePlayerConnection, command);
    }
    else
    {
        qDebug() << "There is no active player connection!";
    }
}

NetworkConnection *NetworkEndpoint::activePlayerConnection()
{
    return m_activePlayerConnection;
}

void NetworkEndpoint::receiveMessages()
{
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

    AbstractCommand* cmd = createCommandForRequest(str);

    emit receivedCommand(cmd);
}

AbstractCommand *NetworkEndpoint::createCommandForRequest(const QString &request)
{
    QJsonDocument json = QJsonDocument::fromJson(request.toUtf8());
    AbstractCommand* cmd;

    int type = json.object()["commandType"].toInt();
    auto timeStamp = AbstractCommand::TimeStampFromJsonValue(json.object()["timeStamp"]);

    switch (type) {
    case Command_ProjectileFired:
        cmd = new ProjectileFiredCommand(timeStamp, json.object()["parameter"].toObject());
        break;
    case Command_PrepareNewGame:
        cmd = new PrepareNewGameCommand(timeStamp, json.object()["parameter"].toObject());
        break;
    case Command_ClientReady:
        cmd = new ClientReadyCommand(timeStamp, json.object()["parameter"].toObject());
        break;
    case Command_Pause:
        cmd = new PauseCommand(timeStamp, json.object()["parameter"].toObject());
        break;
    case Command_ProjectileHit:
        cmd = new ProjectileHitCommand(timeStamp, json.object()["parameter"].toObject());
        break;

    // ...

    default:
        qDebug() << "error parsing client request";
        return nullptr;
    }

    return cmd;
}

void NetworkEndpoint::sendMessage(NetworkConnection *client, AbstractCommand *command)
{
    QJsonDocument message;

    QJsonObject mainObject;

    mainObject.insert("commandType", QJsonValue(command->commandType()));
    mainObject.insert("timeStamp", AbstractCommand::TimeStampToJsonValue(command->timeStamp()));
    mainObject.insert("parameter", QJsonValue(command->toJson()));

    message.setObject(mainObject);

    sendMessage(client, message);
}

void NetworkEndpoint::sendMessage(NetworkConnection* client, QJsonDocument &message) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);

    QString msgString = QString(message.toJson(QJsonDocument::JsonFormat::Compact));

    // message size will go here
    out << (quint16) 0;

    out << msgString;

    // write message size
    out.device()->seek(0);
    out << (quint16) (block.size() - sizeof(quint16));

    client->write(block);
}


} // namespace terminus
