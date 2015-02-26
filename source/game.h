#pragma once

#include <memory>

#include <QObject>
#include <QOpenGLFunctions>
#include <QQuickItem>

#include <deferredactionhandler.h>
#include <eventhandler.h>
#include <util/timer.h>

class QTimer;
class QTime;

namespace terminus
{
class World;

class Game : public QQuickItem
{
    Q_OBJECT

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

    World & world() const;

    DeferredActionHandler & deferredActionHandler();

    Timer & timer();

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
    void keyPressEvent(Qt::Key key);
    void keyReleaseEvent(Qt::Key key);
    void mouseMoveEvent(qreal x, qreal y);
    void touchMoveEvent(qreal x, qreal y);
    void gyroMoveEvent(qreal x, qreal y);
    void flickEvent(qreal startX, qreal x);
    void flickReset();
    void touchChargeFire();
    void touchFire();

    /*!
     * \brief Pause or continue ingame time
     * \param paused Whether the game should be paused or not
     */
    void setPaused(bool paused);
    void togglePaused();

protected:
    std::unique_ptr<World> m_world;

    EventHandler m_eventHandler;
    DeferredActionHandler m_deferredActionHandler;
    Timer m_timer;

    QOpenGLFunctions m_gl;

    std::unique_ptr<QTimer> m_renderTrigger;
    bool m_paused;
    bool m_setupComplete;
};

}
