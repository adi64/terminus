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
 * \brief The NetworkManager implements the basic functionality for the multiplayer mode of this game
 *
 * It provides an interface to initiate network connections, send commands and schedule received commands for execution.
 *
 * \sa NetworkEndpoint
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
    explicit NetworkManager(Game & game);
    virtual ~NetworkManager();

    /*!
     * \brief starts this NetworkManager in Mode::MultiplayerHost and creates the respective NetworkEndpoint
     * \param port
     *
     * \sa NetworkServer
     */
    void startServer(unsigned short port);
    /*!
     * \brief starts this NetworkManager in Mode::MultiplayerClient and creates the respective NetworkEndpoint
     * \param host
     * \param port
     *
     * \sa NetworkClient
     */
    void startClient(QString host, unsigned short port);

    /*!
     * \brief checks the sync timer and sends a sync command as necessary
     *
     * This method is meant to be called by the associated game during its frame update
     */
    void update();


    void sendPauseCommand(bool pause);
    void sendPrepareNewGameCommand(bool isPlayerOne, unsigned int terrainSeed);
    void sendProjectileFiredCommand(QVector3D startPosition, QVector3D velocity);
    void sendProjectileHitCommand(int wagonIndex, float damage);
    void sendSyncCommand(const Train &playerTrain);
    void sendGameEndedCommand(bool youWin);
    void sendClientReadyCommand();

    /*!
     * \brief Initiates the command sequence of a multiplayer game
     *
     * Gets called if a NetworServer goes from State::Listening to State::Connected
     * and is therefore only relevant in Mode::MultiplayerHost
     */
    void prepareAndSyncNewGame();

    /*!
     * \return operation mode of this network manager
     *
     * Before either startClient() or startServer() the mode is Mode::Singleplayer
     */
    Mode mode();

signals:
    /*!
     * \brief sends a command
     * \param command - takes ownership
     *
     * This method may be called from any thread and uses a QueuedConnection to
     * route this request to the network thread
     *
     * \sa onSendCommand()
     */
    void sendCommand(AbstractCommand * command);

protected:
    /*!
     * \brief sets a new NetworkEndpoint and connects its signals
     * \param endpoint - ownership managed by QObject parent mechanism
     *
     * The previous NetworkEndpoints signals will be disconnected
     */
    void setEndpoint(NetworkEndpoint * endpoint);

protected slots:
    /*!
     * \brief called if the underlying NetworkEndpoint receives an AbstractCommand
     * \param command - ownership is taken
     *
     * command is scheduled to be executed for the next frame update
     */
    void onReceivedCommand(AbstractCommand * command);

    /*!
     * \brief called if the underlying NetworkEndpoints changes its state
     * \param state
     *
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
    /*!
     * \brief Game instance that this NetworkManager is associated to
     */
    Game & m_game;

    /*!
     * \brief operation mode of this NetworkManager
     */
    Mode m_mode;

    /*!
     * \brief NetworkEndpoint that is used for low-level network communication
     *
     * A pointer is necessary as specific subclasses of NetworkEndpoint may be used
     */
    std::unique_ptr<NetworkEndpoint> m_networkEndpoint;

    /*!
     * \brief timer that determines when sync commands should be sent
     */
    Timer::TimerID m_syncTimer;
};

} // namespace terminus
