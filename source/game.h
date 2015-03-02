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
    Timer & timer();

    Q_INVOKABLE void buttonInput(int type);
    Q_INVOKABLE void keyInput(Qt::Key key);
    Q_INVOKABLE void moveInput(int type, qreal x, qreal y);

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
    void updateQMLData();

    std::unique_ptr<World> m_world;

    EventHandler m_eventHandler;
    DeferredActionHandler m_deferredActionHandler;
    Timer m_timer;

    QVariant m_qmlData;
    QOpenGLFunctions m_gl;

    std::unique_ptr<QTimer> m_renderTrigger;
    bool m_paused;
    bool m_setupComplete;
};

}
