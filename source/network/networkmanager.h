#pragma once

#include <memory>

#include <QObject>
#include <QString>

#include <network/networkendpoint.h>

namespace terminus
{

class AbstractCommand;
class Game;
class NetworkClient;
class NetworkServer;

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    NetworkManager(Game &game);

    void startServer(unsigned short port);
    void startClient(QString host, unsigned short port);

    void sendMessage(AbstractCommand *command);

    void sendPauseCommand(bool pause);
    void sendPrepareNewGameCommand(bool isPlayerOne, unsigned int terrainSeed);
    void sendProjectileFiredCommand(QVector3D startPosition, QVector3D velocity);
    void sendProjectileHitCommand(int wagonIndex, float damage);
    void sendClientReadyCommand();

    void clientReady();

    bool isClient() const;
    bool isServer() const;
    bool isConnected() const;

    QString localIPAddress() const;

    NetworkServer *networkServer() const;
    NetworkClient *networkClient() const;

protected slots:
    void newCommand(AbstractCommand* command);
    void prepareAndSyncNewGame();

protected:
    Game & m_game;
    std::unique_ptr<NetworkEndpoint> m_networkEndpoint;

    enum EndpointType
    {
        SERVER,
        CLIENT,
        INVALID
    } m_endpointType;
};

} // namespace terminus
