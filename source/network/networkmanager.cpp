#include "networkmanager.h"

#include <assert.h>

#include <game.h>
#include <world/world.h>
#include <world/drawables/terrain.h>

#include <util/timer.h>

#include <network/commands/abstractcommand.h>
#include <network/commands/preparenewgamecommand.h>
#include <network/networkconnection.h>
#include <network/networkclient.h>
#include <network/networkendpoint.h>
#include <network/networkserver.h>

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
    server->start();

    if(m_networkEndpoint)
    {
        disconnect(m_networkEndpoint.get(), &NetworkEndpoint::receivedCommand, this, &NetworkManager::newCommand);
        disconnect(m_networkEndpoint.get(), &NetworkEndpoint::prepareAndSyncNewGame, this, &NetworkManager::prepareAndSyncNewGame);
    }
    connect(server, &NetworkEndpoint::receivedCommand, this, &NetworkManager::newCommand);
    connect(server, &NetworkServer::prepareAndSyncNewGame, this, &NetworkManager::prepareAndSyncNewGame);

    m_networkEndpoint.reset(server);
    m_endpointType = EndpointType::SERVER;
}

void NetworkManager::startClient(QString host, unsigned short port)
{
    auto client = new NetworkClient;
    client->connectToHost(host, port);

    if(m_networkEndpoint)
    {
        disconnect(m_networkEndpoint.get(), &NetworkEndpoint::receivedCommand, this, &NetworkManager::newCommand);
        disconnect(m_networkEndpoint.get(), &NetworkEndpoint::prepareAndSyncNewGame, this, &NetworkManager::prepareAndSyncNewGame);
    }
    connect(client, &NetworkEndpoint::receivedCommand, this, &NetworkManager::newCommand);

    m_networkEndpoint.reset(client);
    m_endpointType = EndpointType::CLIENT;
}

void NetworkManager::sendMessage(AbstractCommand *command)
{
    if(isConnected())
    {
        m_networkEndpoint->sendMessage(command);
    }
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

    qDebug() << "ermergerd new command!";
    command->setGame(&m_game);
    m_game.world().scheduleAction( [=](){ command->run(); delete command; return false; } );
}

void NetworkManager::prepareAndSyncNewGame()
{
    m_game.deferredActionHandler().scheduleAction(
        [&]()
        {
            m_game.startNetworkGame(true);

            // assume that a client is always second player

            auto command = PrepareNewGameCommand(true, m_game.world().terrain().seed());
            sendMessage(&command);

            return false;
        }
    );
}

} // namespace terminus
