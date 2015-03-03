#include "networkmanager.h"

#include <assert.h>

#include <game.h>
#include <world/world.h>
#include <network/commands/abstractcommand.h>
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

    disconnect(m_networkEndpoint.get(), &NetworkEndpoint::receivedCommand, this, &NetworkManager::newCommand);
    connect(server, &NetworkEndpoint::receivedCommand, this, &NetworkManager::newCommand);

    m_networkEndpoint.reset(server);
    m_endpointType = EndpointType::SERVER;
}

void NetworkManager::startClient(QString host, unsigned short port)
{
    auto client = new NetworkClient;
    client->connectToHost(host, port);

    disconnect(m_networkEndpoint.get(), &NetworkEndpoint::receivedCommand, this, &NetworkManager::newCommand);
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

} // namespace terminus
