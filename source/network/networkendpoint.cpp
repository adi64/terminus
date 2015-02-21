#include "networkendpoint.h"

#include <QJsonDocument>

#include "networkconnection.h"

namespace terminus
{

void NetworkEndpoint::handleNewCommands()
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
    socket->setParent(cmd); // delete connection object when command is completed

    cmd->run();
}

AbstractCommand *NetworkEndpoint::createCommandForRequest(const QString &request)
{
    QJsonDocument json = QJsonDocument::fromJson(request.toUtf8());
    AbstractCommand* cmd;

    int type = json.object()["commandType"].toInt();
    switch (type) {
    case Commands::Command_Hello:
        cmd = new TestCommand(clientConnection, m_clInt);
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
