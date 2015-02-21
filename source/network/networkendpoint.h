#pragma once

#include <QObject>
#include <QString>

namespace terminus {

class AbstractCommand;

class NetworkEndpoint : public QObject
{
    Q_OBJECT

public:


protected:
    void receiveMessages();
    void sendMessage(NetworkConnection *client, QJsonDocument &message);

    AbstractCommand *createCommandForRequest(const QString &request);
};

}
