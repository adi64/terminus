#pragma once

#include <QObject>

#include <network/networkendpoint.h>

class QTcpServer;

namespace terminus
{

/*!
 * \brief The NetworkServer implements the NetworkEndpoint interface
 * as the server role of a TCP connection
 *
 * \sa NetworkEndpoint
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
