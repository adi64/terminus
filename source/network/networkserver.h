#pragma once

#include <QObject>
#include <QHostAddress>

#include <network/networkendpoint.h>
#include <util/timer.h>

#include <world/drawables/train/wagons/abstractwagon.h>

class QTcpServer;

namespace terminus
{

/*!
 * \brief The NetworkServer holds two NetworkEndpoints that each represent a
 * connected client.
 *
 * TODO FIXME: In the current implementation, the NetworkServer is a singleton!
 *
 * \sa NetworkEndpoint
 */
class NetworkServer : public QObject
{
    Q_OBJECT
public:
    virtual ~NetworkServer();

    // TODO FIXME singleton instance getter
    static NetworkServer * instance();

    bool listen(const QHostAddress &address = QHostAddress::Any, uint16_t port = 0);

protected slots:
    void onClientConnected();
    void onClientDisconnected();
    void onReceivedCommand(AbstractCommand * command);

protected:
    // TODO FIXME singleton constructor
    explicit NetworkServer(QObject* parent = nullptr);

    // TODO FIXME singleton instance ptr
    static NetworkServer * s_instance;

    void prepareNewGame();

    QTcpServer * m_server;

    std::unique_ptr<NetworkEndpoint> m_clientEndpoints[2];
    std::vector<WagonType> m_trainConfigurations[2];
    bool m_clientReadyFlags[2];
    unsigned int m_terrainSeed;
    Timer m_timer;

};

} //namespace terminus
