#pragma once

#include <memory>

#include <QObject>
#include <QOpenGLFunctions>
#include <QQuickItem>

#include <deferredactionhandler.h>
#include <eventhandler.h>
#include <network/networkmanager.h>
#include <util/timer.h>

class QTimer;
class QTime;
class QVariant;

namespace terminus
{

class World;

const unsigned short defaultPort = 7331;


class Game : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QVariant qmlData READ qmlData NOTIFY qmlDataChanged())

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

    Q_INVOKABLE void startLocalGame();
    Q_INVOKABLE void hostNetworkGame();
    Q_INVOKABLE void joinNetworkGame(QString host);

    void createWorld(bool isNetworkGame, bool isPlayerOne, int terrainSeed);

    void toggleUI();
    void showUI();
    void hideUI();

    World & world() const;
    QVariant & qmlData();
    DeferredActionHandler & deferredActionHandler();

    NetworkManager & networkManager();

    Timer & timer();

    Q_INVOKABLE void buttonInput(int type);
    Q_INVOKABLE void keyInput(Qt::Key key);
    Q_INVOKABLE void moveInput(int type, qreal x, qreal y);
    Q_INVOKABLE void touchInput(qreal oldx, qreal oldy, qreal x, qreal y);

public slots:
    /*!
     * \brief Update game world, taking elapsed time into account
     *
     * This updates all dynamic elements in the game.
     * All calculation should be done in this step so that this method transforms one valid game state into another one.
     */
    void sync();

    /*!
     * \brief Render game world
     *
     * This renders the current state of all objects. Object state should not be changed here.
     * \sa sync()
     */
    void render();
    void cleanup();
    void handleWindowChanged(QQuickWindow* win);

    /*!
     * \brief Pause or continue ingame time
     * \param paused Whether the game should be paused or not
     */
    void setPaused(bool paused);
    void togglePaused();
    bool isPaused() { return m_timer.isPaused(); }

signals:
    void qmlDataChanged();

protected:
    void updateQMLData();

    std::unique_ptr<World> m_world;

    EventHandler m_eventHandler;
    DeferredActionHandler m_deferredActionHandler;
    NetworkManager m_networkManager;
    Timer m_timer;

    QVariant m_qmlData;
    QOpenGLFunctions m_gl;

    std::unique_ptr<QTimer> m_renderTrigger;

    bool m_isPlayerOne;
    bool m_uiIsActive;
};

}
