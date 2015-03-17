#pragma once

#include <memory>

#include <QObject>
#include <QTcpServer>

#include <network/networkendpoint.h>

namespace terminus
{

class AbstractCommand;
class NetworkConnection;

/*!
 * \brief The NetworkServer class provides a server implementation of the
 * NetworkEndpoint interface.
 */
class NetworkServer : public NetworkEndpoint
{
    Q_OBJECT
public:
    NetworkServer(QObject* parent = 0);
    virtual ~NetworkServer();

    bool listen(unsigned short port);

protected slots:
    void onClientConnected();
    void onClientDisconnected();

protected:
    QTcpServer * m_server;
};

} //namespace terminus
