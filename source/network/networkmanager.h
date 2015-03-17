#pragma once

#include <memory>

#include <QObject>
#include <QString>

#include <util/timer.h>

#include <network/networkendpoint.h>

namespace terminus
{

class AbstractCommand;
class Game;
class NetworkClient;
class NetworkServer;
class Train;

/*!
 * \brief The NetworkManager class provides the highest level of abstraction
 * regarding network functionality.
 *
 * It provides methods to send network commands to the other connected game,
 * if any. It can start a network server or client and then provides a connection to the other player.
 *
 * \sa NetworkEndpoint and NetworkConnection
 */
class NetworkManager : public QObject
{
    Q_OBJECT
public:
    enum class Mode
    {
        Singleplayer,
        MultiplayerHost,
        MultiplayerClient
    };

public:
    NetworkManager(Game & game);
    virtual ~NetworkManager();

    void startServer(unsigned short port);
    void startClient(QString host, unsigned short port);

    bool isConnected() const;

    void sendPauseCommand(bool pause);
    void sendPrepareNewGameCommand(bool isPlayerOne, unsigned int terrainSeed);
    void sendProjectileFiredCommand(QVector3D startPosition, QVector3D velocity);
    void sendProjectileHitCommand(int wagonIndex, float damage);
    void sendPrimaryActionCommand(unsigned int selectedWagonIndex, QVector3D aimDirection);
    void sendSyncCommand(const Train &playerTrain);
    void sendGameEndedCommand(bool firstPlayerWon);
    void sendClientReadyCommand();

    /*!
     * \brief Unpauses the game when the client is ready.
     *
     * Gets called when the client signals that it successfully created the
     * World using the provided terrain seed and now is ready to start the game
     */
    void clientReady();
    /*!
     * \brief Send game info to client and pause game
     */
    void prepareAndSyncNewGame();

    Mode mode();

signals:
    void sendCommand(AbstractCommand * command);

public slots:
    void update();

protected:
    void setEndpoint(NetworkEndpoint * endpoint);

protected slots:
    /*!
     * \brief called if the underlying NetworkEndpoint receives a Command
     * \param command a Pointer to the received command
     * command is scheduled to be executed for the next frame update
     */
    void onReceivedCommand(AbstractCommand * command);

    /*!
     * \brief called if the underlying NetworkEndpoints gets or looses its connection
     * \param state
     * This only reacts to a client connecting to a Server and prepares the multiplayer session accordingly
     */
    void onEndpointStateChange(NetworkEndpoint::State state);

    /*!
     * \brief Slot-wrapper for sendMessage()
     * \param command A pointer to the command to be sent
     *
     * A "wrapper" through a signal-slot-connection is used here to prevent
     * calling methods on the underlying socket from a thread different from
     * the one that created the socket. By using Qt::AutoConnection, messages
     * are queued and guaranteed to execute in the right thread.
     */
    void onSendCommand(AbstractCommand* command);

protected:
    Game & m_game;

    Mode m_mode;

    /*!
     * \brief The NetworkEndpoint is stored as a pointer because it will be
     * replaced dynamically (e.g. when stopping server and connecting as
     * client)
     */
    std::unique_ptr<NetworkEndpoint> m_networkEndpoint;

    /*!
     * \brief The timer that determines when sync messages are sent
     */
    Timer::TimerID m_syncTimer;
};

} // namespace terminus
