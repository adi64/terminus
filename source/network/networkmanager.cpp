#include "networkmanager.h"

#include <cassert>

#include <game.h>
#include <network/commands/abstractcommand.h>
#include <network/commands/preparenewgamecommand.h>
#include <network/commands/clientreadycommand.h>
#include <network/commands/projectilefiredcommand.h>
#include <network/commands/projectilehitcommand.h>
#include <network/commands/primaryactioncommand.h>
#include <network/commands/pausecommand.h>
#include <network/commands/synccommand.h>
#include <network/commands/gameendedcommand.h>
#include <network/networkclient.h>
#include <network/networkendpoint.h>
#include <network/networkserver.h>
#include <util/timer.h>
#include <world/world.h>
#include <world/drawables/terrain.h>
#include <world/drawables/train/train.h>



namespace terminus
{

NetworkManager::NetworkManager(Game &game)
: m_game(game)
, m_mode(Mode::Singleplayer)
, m_networkEndpoint(nullptr)
, m_syncTimer(0)
{
    connect(this, &NetworkManager::sendCommand, this, &NetworkManager::onSendCommand);
}

NetworkManager::~NetworkManager()
{
    m_game.timer().releaseTimer(m_syncTimer);
}

void NetworkManager::startServer(unsigned short port)
{
    auto server = new NetworkServer;

    setEndpoint(server);

    m_mode = Mode::MultiplayerHost;

    server->listen(port);
}

void NetworkManager::startClient(QString host, unsigned short port)
{
    auto client = new NetworkClient;

    setEndpoint(client);

    m_mode = Mode::MultiplayerClient;

    client->connectClient(host, port);
}

void NetworkManager::update()
{
    // lazy initialize
    if(m_syncTimer == 0)
    {
        m_syncTimer = m_game.timer().allocateTimer();
    }

    auto timeSinceLastSync = m_game.timer().get(m_syncTimer);
    if(timeSinceLastSync > 500)
    {
        // send sync command
        sendSyncCommand(m_game.world().localPlayerTrain());

        m_game.timer().adjust(m_syncTimer, 0);
    }
}

void NetworkManager::setEndpoint(NetworkEndpoint * endpoint)
{
    if(m_networkEndpoint)
    {
        disconnect(m_networkEndpoint.get(), &NetworkEndpoint::receivedCommand, this, &NetworkManager::onReceivedCommand);
        disconnect(m_networkEndpoint.get(), &NetworkEndpoint::stateChanged, this, &NetworkManager::onEndpointStateChange);
    }
    m_networkEndpoint.reset(endpoint);
    connect(m_networkEndpoint.get(), &NetworkEndpoint::receivedCommand, this, &NetworkManager::onReceivedCommand);
    connect(m_networkEndpoint.get(), &NetworkServer::stateChanged, this, &NetworkManager::onEndpointStateChange);
}

void NetworkManager::sendPauseCommand(bool pause)
{
    auto command = new PauseCommand(m_game.timer().get(), pause);
    emit sendCommand(command);
}

void NetworkManager::sendPrepareNewGameCommand(bool isPlayerOne, unsigned int terrainSeed)
{
    auto command = new PrepareNewGameCommand(m_game.timer().get(), isPlayerOne, terrainSeed);
    emit sendCommand(command);
}

void NetworkManager::sendProjectileFiredCommand(QVector3D startPosition, QVector3D velocity)
{
    auto command = new ProjectileFiredCommand(m_game.timer().get(), startPosition, velocity);
    emit sendCommand(command);
}

void NetworkManager::sendProjectileHitCommand(int wagonIndex, float damage)
{
    auto command = new ProjectileHitCommand(m_game.timer().get(), true, wagonIndex, damage);
    emit sendCommand(command);
}

void NetworkManager::sendPrimaryActionCommand(unsigned int selectedWagonIndex, QVector3D aimDirection)
{
    auto command = new PrimaryActionCommand(m_game.timer().get(), selectedWagonIndex, aimDirection);
    emit sendCommand(command);
}

void NetworkManager::sendSyncCommand(const Train &playerTrain)
{
    auto command = new SyncCommand(m_game.timer().get(), playerTrain);
    emit sendCommand(command);
}

void NetworkManager::sendGameEndedCommand(bool firstPlayerWon)
{
    auto command = new GameEndedCommand(m_game.timer().get(), firstPlayerWon);
    emit sendCommand(command);
}

void NetworkManager::sendClientReadyCommand()
{
    auto command = new ClientReadyCommand(m_game.timer().get());
    emit sendCommand(command);
}

void NetworkManager::prepareAndSyncNewGame()
{
    // assume that a client is always second player
    sendPrepareNewGameCommand(false, m_game.world().level().config().seed());

    // pause game
    sendPauseCommand(true);
    m_game.setPaused(true);
}

NetworkManager::Mode NetworkManager::mode()
{
    return m_mode;
}

void NetworkManager::clientReady()
{
    // unpause both clients
    sendPauseCommand(false);
    m_game.setPaused(false);
}

bool NetworkManager::isConnected() const
{
    if(m_mode == Mode::Singleplayer)
    {
        return false;
    }

    assert(m_networkEndpoint != nullptr);

    return m_networkEndpoint->state() == NetworkEndpoint::State::Connected;
}

void NetworkManager::onReceivedCommand(AbstractCommand * command)
{
    if(!command || m_mode == Mode::Singleplayer)
    {
        return;
    }

    command->setGame(&m_game);
    m_game.scheduler().scheduleAction( [=](){ command->run(); delete command; return false; } );
}

void NetworkManager::onEndpointStateChange(NetworkEndpoint::State state)
{
    if(state == NetworkEndpoint::State::Connected && m_mode == Mode::MultiplayerHost)
    {
        prepareAndSyncNewGame();
    }
}

void NetworkManager::onSendCommand(AbstractCommand * command)
{
    if(!command || m_mode == Mode::Singleplayer)
    {
        return;
    }

    assert(m_networkEndpoint);
    m_networkEndpoint->sendCommand(command);

    delete command;
}

} // namespace terminus
