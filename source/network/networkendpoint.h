#pragma once

#include <QObject>

class QString;
class QTcpSocket;

namespace terminus {

class AbstractCommand;

/*!
 * \brief The NetworkEndpoint class provides a common interface that NetworkServer and NetworkClient implement.
 *
 * It manages an underlying TcpSocket and implements logic to serialize and deserialize AbstractCommands
 *
 * \sa NetworkServer, NetworkClient, AbstractCommand
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

    /*!
     * \brief serializes command and sends it to the opposite NetworkEndpoint
     * \param command - ownership is not taken
     */
    void sendCommand(AbstractCommand * command);

    /*!
     * \brief disconnect the underlying socket
     */
    void disconnect();

    /*!
     * \return current state of this NetworkEndpoint
     */
    State state();

signals:
    /*!
     * \brief fired for every AbstractCommand received from the opposite NetworkEndpoint
     * \param command - ownership passes to receiver of this signal
     */
    void receivedCommand(AbstractCommand * command);

    /*!
     * \brief fired after state of this NetworkEndpoint is changed
     * \param newState
     */
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
