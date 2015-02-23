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
    NetworkManager(std::shared_ptr<Game> game);

    void startServer(unsigned short port);
    void startClient(QString host, unsigned short port);

    bool isClient() const;
    bool isServer() const;

    NetworkServer *networkServer() const;
    NetworkClient *networkClient() const;

protected slots:
    void newCommand(AbstractCommand* command);

protected:
    std::shared_ptr<Game> m_game;
    std::unique_ptr<NetworkEndpoint> m_networkEndpoint;

    enum EndpointType
    {
        SERVER,
        CLIENT,
        INVALID
    } m_endpointType;
};

} // namespace terminus
