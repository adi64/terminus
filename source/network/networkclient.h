#pragma once

#include <memory>

#include <QObject>

#include <network/networkendpoint.h>

namespace terminus
{

class NetworkConnection;

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
