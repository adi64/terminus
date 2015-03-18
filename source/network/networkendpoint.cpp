#include "networkendpoint.h"

#include <cassert>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QTcpSocket>
#include <QtEndian>

#include <network/commands/abstractcommand.h>
#include <network/commands/clientreadycommand.h>
#include <network/commands/gameendedcommand.h>
#include <network/commands/pausecommand.h>
#include <network/commands/preparenewgamecommand.h>
#include <network/commands/projectilefiredcommand.h>
#include <network/commands/projectilehitcommand.h>
#include <network/commands/synccommand.h>

namespace terminus
{

NetworkEndpoint::NetworkEndpoint(QObject *parent)
: QObject(parent)
, m_state(State::Disconnected)
, m_socket(nullptr)
{
}

NetworkEndpoint::~NetworkEndpoint()
{
    disconnect();
}

void NetworkEndpoint::sendCommand(AbstractCommand * command)
{
    sendMessage(serializeCommand(command));
}

void NetworkEndpoint::disconnect()
{
    if(socket())
    {
        socket()->disconnectFromHost();
    }
}

NetworkEndpoint::State NetworkEndpoint::state()
{
    return m_state;
}

QTcpSocket * NetworkEndpoint::socket()
{
    return m_socket;
}

void NetworkEndpoint::setSocket(QTcpSocket * socket)
{
    m_socket = socket;
}

void NetworkEndpoint::enterState(NetworkEndpoint::State state)
{
    m_state = state;
    emit stateChanged(m_state);
}

QString NetworkEndpoint::serializeCommand(AbstractCommand * command)
{
    QJsonDocument message;
    QJsonObject mainObject;

    mainObject.insert("commandType", QJsonValue(command->commandType()));
    mainObject.insert("timeStamp", QJsonValue(static_cast<double>(command->timeStamp())));
    mainObject.insert("parameter", QJsonValue(command->toJson()));
    message.setObject(mainObject);

    return QString(message.toJson(QJsonDocument::JsonFormat::Compact));
}

AbstractCommand * NetworkEndpoint::deserializeCommand(const QString & message)
{
    QJsonDocument json = QJsonDocument::fromJson(message.toUtf8());
    AbstractCommand* cmd;

    int type = json.object()["commandType"].toInt();
    auto timeStamp = static_cast<Timer::TimerMSec>(json.object()["timeStamp"].toDouble());

    switch (type) {
    case Command_Pause:
        cmd = new PauseCommand(timeStamp, json.object()["parameter"].toObject());
        break;
    case Command_PrepareNewGame:
        cmd = new PrepareNewGameCommand(timeStamp, json.object()["parameter"].toObject());
        break;
    case Command_ProjectileFired:
        cmd = new ProjectileFiredCommand(timeStamp, json.object()["parameter"].toObject());
        break;
    case Command_ProjectileHit:
        cmd = new ProjectileHitCommand(timeStamp, json.object()["parameter"].toObject());
        break;
    case Command_ClientReady:
        cmd = new ClientReadyCommand(timeStamp, json.object()["parameter"].toObject());
        break;
    case Command_Sync:
        cmd = new SyncCommand(timeStamp, json.object()["parameter"].toObject());
        break;
    case Command_GameEnded:
        cmd = new GameEndedCommand(timeStamp, json.object()["parameter"].toObject());
        break;
        //...
    default:
        return nullptr;
    }

    return cmd;
}

void NetworkEndpoint::sendMessage(const QString & message)
{
    if(m_state != State::Connected)
    {
        qDebug() << "Can not send message as there is no open connection.";
        return;
    }
    assert(socket());
    QTcpSocket & tcpSocket = *socket();

    qDebug() << "send:" << message;

    QByteArray utf8Message = message.toUtf8();

    uchar lengthBuffer[2];
    qToBigEndian<quint16>(utf8Message.size(), lengthBuffer);
    tcpSocket.write((char*)lengthBuffer, 2);

    tcpSocket.write(utf8Message);
}

void NetworkEndpoint::onDataReceived()
{
    assert(socket());
    QTcpSocket & tcpSocket = *socket();

    while(true)
    {
        if (tcpSocket.bytesAvailable() < 4)
        {
            break;
        }

        uchar buffer[2];
        tcpSocket.peek((char*)buffer, 2);
        unsigned short messageLength = qFromBigEndian<quint16>(buffer);

        if (tcpSocket.bytesAvailable() < messageLength)
        {
            break;
        }

        tcpSocket.read(2);

        QByteArray utf8Message = tcpSocket.read(messageLength);
        AbstractCommand * command = deserializeCommand(QString::fromUtf8(utf8Message));
        if(command)
        {
            emit receivedCommand(command);
        }
    }
}

} // namespace terminus
