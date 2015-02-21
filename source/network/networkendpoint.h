#pragma once

#include <QObject>
#include <QString>

namespace terminus {

class AbstractCommand;
class NetworkConnection;

class NetworkEndpoint : public QObject
{
    Q_OBJECT

public:
    NetworkEndpoint(QObject * parent = 0);

protected:
    void receiveMessages();
    void sendMessage(NetworkConnection *client, QJsonDocument &message);

    AbstractCommand *createCommandForRequest(const QString &request);

    quint16 m_expectedMessageSize;
};

}
