#pragma once

#include <memory>

#include <QObject>
#include <QQuickItem>

#include <bullet/btBulletDynamicsCommon.h>

#include <world/scene.h>
#include <world/drawables/train/train.h>

#include "userinterface.h"

class QTimer;
class QTime;

namespace terminus
{

class Terrain;
class ResourceManager;
class SkyBox;
class DeferredActionHandler;
class AbstractPlayer;
class AIPlayer;
class LocalPlayer;

class Game : public QQuickItem
{
    Q_OBJECT

public:
    Game();

    /*!
     * Do not delete this destructor, even if it is empty
     * - otherwise std::shared_ptr<IncompleteType> in the header will break
     */
    ~Game();

    Scene *scene() const;
    Train *playerTrain() const;
    Train *enemyTrain() const;
    AbstractPlayer *localPlayer() const;

    Q_INVOKABLE UserInterface *userInterface();

    void btTickCallback(btDynamicsWorld *world, btScalar timeStep);
    static void btStaticTickCallback(btDynamicsWorld *world, btScalar timeStep);

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
    void render() const;
    void cleanup();
    void handleWindowChanged(QQuickWindow* win);

    /*!
     * \brief Pause or continue ingame time
     * \param paused Whether the game should be paused or not
     */
    void setPaused(bool paused);
    void togglePaused();

signals:
    void gameSyncCompleted();

protected:
    void setupBulletWorld(void);

    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Train> m_playerTrain;
    std::shared_ptr<Train> m_enemyTrain;
    std::unique_ptr<LocalPlayer> m_localPlayer;
    std::unique_ptr<AIPlayer> m_aiPlayer;
    std::unique_ptr<QTimer> m_timer;
    std::shared_ptr<QTime> m_timeStamp;
    std::shared_ptr<DeferredActionHandler> m_deferredActionHandler;
    std::unique_ptr<Terrain> m_terrain;
    std::unique_ptr<SkyBox> m_skybox;

    std::unique_ptr<UserInterface> m_userinterface;

    bool m_paused;
    bool m_setupComplete;

    // bullet
    // these objects must not be deleted before m_bullet_dynamicsWorld
    // -- so as a temporary hack, we won't delete them at all
    btBroadphaseInterface* m_bullet_broadphase;
    btDefaultCollisionConfiguration* m_bullet_collisionConfiguration;
    btCollisionDispatcher* m_bullet_dispatcher;
    btSequentialImpulseConstraintSolver* m_bullet_solver;

    std::shared_ptr<btDiscreteDynamicsWorld> m_bullet_dynamicsWorld;
};

}
