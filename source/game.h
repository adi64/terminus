#pragma once

#include <memory>

#include <QObject>
#include <QQuickItem>

#include "bullet/btBulletDynamicsCommon.h"

#include "scene.h"
#include "train.h"

class QTimer;
class QTime;

namespace terminus
{

class Terrain;
class ResourceManager;
class SkyBox;
class DeferredActionHandler;

class Game : public QQuickItem
{
    Q_OBJECT

public:
    Game();
    ~Game();

    Scene *scene() const;
    Train *playerTrain() const;

    void btTickCallback(btDynamicsWorld *world, btScalar timeStep);
    static void btStaticTickCallback(btDynamicsWorld *world, btScalar timeStep);
public slots:
    void sync();
    void render();
    void cleanup();
    void handleWindowChanged(QQuickWindow* win);

protected:
    void setupBulletWorld(void);

    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Train> m_playerTrain;
    std::shared_ptr<Train> m_enemyTrain;
    std::unique_ptr<QTimer> m_timer;
    std::shared_ptr<QTime> m_timeStamp;
    std::shared_ptr<DeferredActionHandler> m_deferredActionHandler;
    std::unique_ptr<Terrain> m_terrain;
    std::unique_ptr<SkyBox> m_skybox;

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
