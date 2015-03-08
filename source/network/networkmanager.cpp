#include "networkmanager.h"

#include <assert.h>

#include <game.h>
#include <world/world.h>
#include <world/drawables/terrain.h>

#include <util/timer.h>

#include <network/commands/abstractcommand.h>
#include <network/commands/preparenewgamecommand.h>
#include <network/commands/clientreadycommand.h>
#include <network/commands/projectilefiredcommand.h>
#include <network/commands/projectilehitcommand.h>
#include <network/commands/primaryactioncommand.h>
#include <network/commands/pausecommand.h>
#include <network/networkconnection.h>
#include <network/networkclient.h>
#include <network/networkendpoint.h>
#include <network/networkserver.h>

#include <QDebug>

namespace terminus
{

NetworkManager::NetworkManager(Game &game)
    : m_game(game)
    , m_networkEndpoint(nullptr)
    , m_endpointType(EndpointType::INVALID)
{

}

void NetworkManager::startServer(unsigned short port)
{
    auto server = new NetworkServer;
    server->setListenPort(port);

    if(m_networkEndpoint)
    {
        disconnect(m_networkEndpoint.get(), &NetworkEndpoint::receivedCommand, this, &NetworkManager::newCommand);
        disconnect(m_networkEndpoint.get(), &NetworkEndpoint::prepareAndSyncNewGame, this, &NetworkManager::prepareAndSyncNewGame);
    }
    connect(server, &NetworkEndpoint::receivedCommand, this, &NetworkManager::newCommand);
    connect(server, &NetworkServer::prepareAndSyncNewGame, this, &NetworkManager::prepareAndSyncNewGame);

    m_networkEndpoint.reset(server);
    m_endpointType = EndpointType::SERVER;

    server->start();
}

void NetworkManager::startClient(QString host, unsigned short port)
{
    auto client = new NetworkClient;

    if(m_networkEndpoint)
    {
        disconnect(m_networkEndpoint.get(), &NetworkEndpoint::receivedCommand, this, &NetworkManager::newCommand);
        disconnect(m_networkEndpoint.get(), &NetworkEndpoint::prepareAndSyncNewGame, this, &NetworkManager::prepareAndSyncNewGame);
    }
    connect(client, &NetworkEndpoint::receivedCommand, this, &NetworkManager::newCommand);

    m_networkEndpoint.reset(client);
    m_endpointType = EndpointType::CLIENT;

    client->connectToHost(host, port);
}

void NetworkManager::sendMessage(AbstractCommand *command)
{
    if(isConnected())
    {
        m_networkEndpoint->sendMessage(command);
    }
}

void NetworkManager::sendPauseCommand(bool pause)
{
    auto command = PauseCommand(m_game.timer().get(), pause);
    sendMessage(&command);
}

void NetworkManager::sendPrepareNewGameCommand(bool isPlayerOne, unsigned int terrainSeed)
{
    auto command = PrepareNewGameCommand(m_game.timer().get(), isPlayerOne, terrainSeed);
    sendMessage(&command);
}

void NetworkManager::sendProjectileFiredCommand(QVector3D startPosition, QVector3D velocity)
{
    auto command = ProjectileFiredCommand(m_game.timer().get(), startPosition, velocity);
    sendMessage(&command);
}

void NetworkManager::sendProjectileHitCommand(int wagonIndex, float damage)
{
    auto command = ProjectileHitCommand(m_game.timer().get(), true, wagonIndex, damage);
    sendMessage(&command);
}

void NetworkManager::sendPrimaryActionCommand(unsigned int selectedWagonIndex, QVector3D aimDirection)
{
    auto command = PrimaryActionCommand(m_game.timer().get(), selectedWagonIndex, aimDirection);
    sendMessage(&command);
}

void NetworkManager::sendClientReadyCommand()
{
    auto command = ClientReadyCommand(m_game.timer().get());
    sendMessage(&command);
}

void NetworkManager::clientReady()
{
    // unpause both clients
    qDebug() << __FILE__ << __LINE__ << "Client is ready, unpausing game!";
    sendPauseCommand(false);
    m_game.setPaused(false);
}

bool NetworkManager::isClient() const
{
    return m_endpointType == EndpointType::CLIENT;
}

bool NetworkManager::isServer() const
{
    return m_endpointType == EndpointType::SERVER;
}

bool NetworkManager::isConnected() const
{
    if(m_endpointType == EndpointType::INVALID)
    {
        return false;
    }

    assert(m_networkEndpoint != nullptr);

    if(m_networkEndpoint->activePlayerConnection() != nullptr)
    {
        return m_networkEndpoint->activePlayerConnection()->isConnected();
    }

    return false;
}

QString NetworkManager::localIPAddress() const
{
    assert(m_endpointType != EndpointType::INVALID);
    if(m_networkEndpoint->activePlayerConnection())
    {
        return m_networkEndpoint->activePlayerConnection()->localAddress().toString();
    }
    else
    {
        qDebug() << "There is no active network connection!";
        return QString();
    }
}

NetworkServer *NetworkManager::networkServer() const
{
    assert(isServer());
    return dynamic_cast<NetworkServer*>(m_networkEndpoint.get());
}

NetworkClient *NetworkManager::networkClient() const
{
    assert(isClient());
    return dynamic_cast<NetworkClient*>(m_networkEndpoint.get());
}

void NetworkManager::newCommand(AbstractCommand *command)
{
    if(!command)
    {
        return;
    }

    command->setGame(&m_game);
    m_game.deferredActionHandler().scheduleAction( [=](){ command->run(); delete command; return false; } );
}

void NetworkManager::prepareAndSyncNewGame()
{
    // assume that a client is always second player
    sendPrepareNewGameCommand(false, m_game.world().terrain().seed());

    // pause game
    sendPauseCommand(true);
    m_game.setPaused(true);
}

} // namespace terminus
