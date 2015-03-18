#pragma once

#include <QObject>
#include <QString>

class QTcpSocket;

namespace terminus {

class AbstractCommand;

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
    enum class State
    {
        Listening,
        Connected,
        Disconnected
    };

public:
    NetworkEndpoint(QObject * parent = 0);
    virtual ~NetworkEndpoint();

    void sendCommand(AbstractCommand *command);
    void disconnect();

    State state();

signals:
    void receivedCommand(AbstractCommand * command);
    void stateChanged(State newState);

protected:
    QTcpSocket * socket();
    void setSocket(QTcpSocket * socket);

    void enterState(State state);

    QString serializeCommand(AbstractCommand * command);
    AbstractCommand * deserializeCommand(const QString & message);

    void sendMessage(const QString & message);

protected slots:
    void onDataReceived();

protected:
    State m_state;
    QTcpSocket * m_socket;
};

}
