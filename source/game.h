#pragma once

#include <memory>

#include <QObject>
#include <QOpenGLFunctions>
#include <QQuickItem>

#include <deferredactionhandler.h>
#include <eventhandler.h>

class QTimer;
class QTime;
class QVariant;

namespace terminus
{
class World;

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

    World & world() const;
    QVariant & qmlData();
    DeferredActionHandler & deferredActionHandler();

    Q_INVOKABLE void keyPressEvent(Qt::Key key);
    Q_INVOKABLE void keyReleaseEvent(Qt::Key key);
    Q_INVOKABLE void mouseMoveEvent(qreal x, qreal y);
    Q_INVOKABLE void touchMoveEvent(qreal x, qreal y);
    Q_INVOKABLE void gyroMoveEvent(qreal x, qreal y);
    Q_INVOKABLE void switchToNextWagon();
    Q_INVOKABLE void switchToPreviousWagon();
    Q_INVOKABLE void touchChargeFire();
    Q_INVOKABLE void touchFire();

    /*Q_INVOKABLE void flickEvent(qreal startX, qreal x);
    Q_INVOKABLE void flickReset();*/

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

signals:
    void qmlDataChanged();

protected:
    void initializeQMLData();

    std::unique_ptr<World> m_world;

    EventHandler m_eventHandler;
    DeferredActionHandler m_deferredActionHandler;

    QVariant m_qmlData;
    QOpenGLFunctions m_gl;

    std::unique_ptr<QTimer> m_timer;
    std::unique_ptr<QTime> m_timeStamp;
    bool m_paused;
    bool m_setupComplete;
};

}
