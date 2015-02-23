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

    bool isClient() const;
    bool isServer() const;
    bool isConnected() const;

    NetworkServer *networkServer() const;
    NetworkClient *networkClient() const;

protected slots:
    void newCommand(AbstractCommand* command);

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
