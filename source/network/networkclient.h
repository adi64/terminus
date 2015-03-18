#pragma once

#include <QObject>

#include <network/networkendpoint.h>

namespace terminus
{

class NetworkConnection;

/*!
 * \brief The NetworkServer implements the NetworkEndpoint interface
 * as the client role of a TCP connection
 *
 * \sa NetworkEndpoint
 */
class NetworkClient : public NetworkEndpoint
{
    Q_OBJECT

public:
    NetworkClient(QObject *parent = 0);

    void connectClient(QString host, unsigned short port);

protected slots:
    void onSocketConnected();
    void onSocketDisconnected();
};

} //namespace terminus
