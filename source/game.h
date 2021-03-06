#pragma once

#include <memory>
#include <queue>

#include <QObject>
#include <QOpenGLFunctions>
#include <QQuickItem>

#include <eventhandler.h>
#include <network/networkmanager.h>
#include <util/actionscheduler.h>
#include <util/timer.h>

class QTimer;
class QTime;
class QVariant;

namespace terminus
{

class World;

const unsigned short defaultPort = 7331;

/*!
 * \brief The Game class is the main entry point.
 * It subclasses QQuickItem and handles all communication between QML and C++.
 *
 * The Game class gets instantiated on QML side and thereby handles all
 * communication with the "outside" / QML environment.beforeSynchronizing
 * It connects to the QML signals QQUickItem::beforeSynchronizing() and
 * QQUickItem::beforeRendering() to receive update and render signals.
 * A timer triggers a redraw every 1000 / 60 milliseconds to cap the framerate at 60fps.
 *
 */
class Game : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QVariant qmlData READ qmlData WRITE writeQmlData NOTIFY qmlDataChanged)

public:
    /*!
     * \brief The one and only Game constructor
     */
    Game();

    /*!
     * \brief Delete copy constructor
     */
    Game(const Game & other) = delete;

    /*!
     * \brief Delete assignment operator
     */
    Game & operator=(const Game & other) = delete;

    /*!
     * Do not delete this destructor, even if it is empty
     * - otherwise std::shared_ptr<IncompleteType> in the header will break
     */
    ~Game();

    /*!
     * \brief Sets up a local game against an AI player with the local plaer
     * being the one on the right
     */
    Q_INVOKABLE void startLocalGame();

    /*!
     * \brief Start a paused game and wait for a connecting player
     */
    Q_INVOKABLE void hostNetworkGame();

    /*!
     * \brief Try to connect to the given host and receive game information
     * \param host The IP address of the host
     */
    Q_INVOKABLE void joinNetworkGame(QString host);

    /*!
     * \brief Generic method to create a World
     * \param isNetworkGame Whether the local game is a network game
     * \param isPlayerOne Whether the local player is player one (right track)
     * \param terrainSeed The seed used to initialize the Perlin noise function
     * to generate the terrain
     */
    void createWorld(bool isNetworkGame, bool isPlayerOne, long long terrainSeed);

    void endGame(bool localPlayerWins, bool showMessage);

    /*!
     * \brief Hide or show the UI
     */
    void toggleUI();
    void showUI();
    void hideUI();

    void toggleFPS();

    World & world() const;
    QVariant & qmlData();
    Q_INVOKABLE void writeQmlData(QVariant qmlData);

    ActionScheduler & scheduler();

    NetworkManager & networkManager();

    Timer & timer();

    /*!
     * \brief Handle a pressed button event from the UI
     */
    Q_INVOKABLE void buttonInput(int type);

    /*!
     * \brief Handle a key event
     */
    Q_INVOKABLE void keyInput(Qt::Key key);

    /*!
     * \brief Handle a mouse move event
     */
    Q_INVOKABLE void moveInput(int type, qreal x, qreal y);
    Q_INVOKABLE void touchInput(qreal oldx, qreal oldy, qreal x, qreal y);

public slots:
    /*!
     * \brief Update game world, taking elapsed time into account
     *
     * This updates all dynamic elements in the game.
     * All calculation should be done in this step so that this method
     * transforms one valid game state into another one.
     */
    void sync();

    /*!
     * \brief Render game world
     *
     * This renders the current state of all objects. Object state should not be
     * changed here.
     *
     * \sa sync()
     */
    void render();

    void cleanup();

    /*!
     * \brief Handles a change of the QQuickWindow
     * \param win A Pointer to the QQuickWindow that this QQuickItem is being
     * displayed on
     *
     * This method gets called upon instantiation and whenever the QQuickWindow
     * changes. It (re)connects sync() and render() to the window.
     *
     * \sa sync() and render()
     */
    void handleWindowChanged(QQuickWindow * win);

    /*!
     * \brief Pause or continue ingame time
     * \param paused Whether the game should be paused or not
     */
    void setPaused(bool paused);
    void togglePaused();
    bool isPaused() { return m_timer.isPaused(); }

signals:
    void qmlDataChanged();
    void fpsChanged();

protected:
    void connectSignals(QQuickWindow * win);
    void disconnectSignals();
    /*!
     * \brief Update the data storage used by the UI to display game info
     */
    void updateQMLData();
    float updateFPS();

protected:
    Timer m_timer;
    EventHandler m_eventHandler;
    ActionScheduler m_scheduler;
    NetworkManager m_networkManager;

    QVariant m_qmlData;

    QQuickWindow * m_window;
    std::unique_ptr<QTimer> m_renderTrigger; //! The timer that triggers a redraw
    bool m_isGLInitialized;
    QOpenGLFunctions m_gl;

    bool m_isPlayerOne;
    bool m_isUIActive;
    bool m_showFPS;

    std::queue<long long> m_frameTimes;

    std::unique_ptr<World> m_world;
};

}
