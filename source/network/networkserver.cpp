#include "networkserver.h"

#include <cassert>

#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>

#include <network/commands/settraincommand.h>
#include <network/commands/preparenewgamecommand.h>

namespace terminus
{

// TODO FIXME singleton instance ptr
NetworkServer * NetworkServer::s_instance = nullptr;

NetworkServer::NetworkServer(QObject* parent)
: QObject(parent)
, m_server(new QTcpServer(this))
, m_clientEndpoints{nullptr, nullptr}
, m_clientReadyFlags{false, false}
, m_terrainSeed(rand())
{
    m_trainConfigurations[0].clear();
    m_trainConfigurations[1].clear();
    connect(m_server, &QTcpServer::newConnection, this, &NetworkServer::onClientConnected);
}

void NetworkServer::prepareNewGame()
{
    m_terrainSeed = rand();
}

NetworkServer::~NetworkServer()
{
    m_server->close();
}

// TODO FIXME singleton instance getter
NetworkServer *NetworkServer::instance()
{
    if(!s_instance)
    {
        s_instance = new NetworkServer();
    }
    return s_instance;
}

bool NetworkServer::listen(const QHostAddress &address /* = QHostAddress::Any */, uint16_t port /* = 0 */)
{
    return m_server->listen(address, port);
}

void NetworkServer::onClientConnected()
{
    QTcpSocket* clientSocket = m_server->nextPendingConnection();

    if(m_clientEndpoints[0] && m_clientEndpoints[1])
    {
        // There are already two clients connected!
        clientSocket->close();
        return;
    }

    uint8_t clientID = m_clientEndpoints[0] ? 1 : 0;

    m_clientEndpoints[clientID] = std::unique_ptr<NetworkEndpoint>(new NetworkEndpoint(this, clientSocket));
    m_trainConfigurations[clientID].clear();
    m_clientReadyFlags[clientID] = false;

    connect(m_clientEndpoints[clientID].get(), &NetworkEndpoint::disconnected, this, &NetworkServer::onClientDisconnected);
    connect(m_clientEndpoints[clientID].get(), &NetworkEndpoint::receivedCommand, this, &NetworkServer::onReceivedCommand);

    auto prepareNewGameCommand = new PrepareNewGameCommand(m_timer.get(), clientID == 0, m_terrainSeed);
    m_clientEndpoints[clientID]->sendCommand(prepareNewGameCommand);

}

void NetworkServer::onClientDisconnected()
{
    // TODO FIXME for now, just delete / close all sockets
    m_clientEndpoints[0] = nullptr;
    m_clientEndpoints[1] = nullptr;

    prepareNewGame();
}

void NetworkServer::onReceivedCommand(AbstractCommand *command)
{
    if(!command)
        return;

    //command->setGame(&m_game);
    //m_game.scheduler().scheduleAction( [=](){ command->doWork(); delete command; return false; } );
}

} //namespace terminus
