#pragma once

#include <QObject>
#include <QString>

namespace terminus {

class AbstractCommand;
class NetworkConnection;

/*!
 * \brief The NetworkEndpoint class provides a common interface that NetworkServer and NetworkClient implement.
 *
 * It provides an active NetworkConnection and means to send an AbstractCommand over that NetworkConnection.
 *
 * \sa NetworkServer, NetworkClient and NetworkConnection
 */
class NetworkEndpoint : public QObject
{
    Q_OBJECT

public:
    NetworkEndpoint(QObject * parent = 0);

    void sendMessage(AbstractCommand *command);

    NetworkConnection *activePlayerConnection();

signals:
    void receivedCommand(AbstractCommand *command);
    void prepareAndSyncNewGame();

protected:
    void receiveMessages();

    void sendMessage(NetworkConnection *client, AbstractCommand *command);
    void sendMessage(NetworkConnection *client, QJsonDocument &message);

    AbstractCommand *createCommandForRequest(const QString &request);

    quint16 m_expectedMessageSize;
    NetworkConnection *m_activePlayerConnection;
};

}
