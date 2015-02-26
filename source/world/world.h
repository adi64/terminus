#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <QObject>

#include <bullet/btBulletDynamicsCommon.h>

#include <world/camera.h>
#include <deferredactionhandler.h>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QTime;

class btDiscreteDynamicsWorld;
class btCollisionObject;

namespace terminus
{
class Game;
class Train;
class Terrain;
class SkyBox;
class AbstractPlayer;
class AIPlayer;
class LocalPlayer;

class AbstractGraphicsObject;
class AbstractPhysicsObject;

class World : public QObject
{
    Q_OBJECT
protected:
    static void btStaticTickCallback(btDynamicsWorld * world, btScalar timeStep);

public:
    /*!
     * \brief The one and only World constructor
     * \param game
     */
    World(Game & game);

    /*!
     * \brief Delete copy constructor
     */
    World(const World & other) = delete;

    /*!
     * \brief Delete assignment operator
     */
    World & operator=(const World & other) = delete;

    ~World();

    void update(int elapsedMilliseconds);
    void render(QOpenGLFunctions & gl) const;

    LocalPlayer & localPlayer();
    Train & playerTrain();
    Train & enemyTrain();

    void setInitialTimeStamp(const std::shared_ptr<QTime> &timeStamp);
    void scheduleAction(DeferredAction event);

    void addNode(AbstractGraphicsObject * node);
    void deleteNode(AbstractGraphicsObject * node);

    btDiscreteDynamicsWorld * bulletWorld();
    void addCollisionMapping(const btCollisionObject * collisionObject, AbstractPhysicsObject * graphicsObject);
    void removeCollisionMapping(const btCollisionObject * collisionObject);
    AbstractPhysicsObject * getGraphicsObjectForCollisionObject(const btCollisionObject * collisionObject) const;

protected:
    void setupBullet();
    void deleteBullet();
    void btTickCallback(btDynamicsWorld * world, btScalar timeStep);

protected:
    Game & m_game;

    std::unique_ptr<Terrain> m_terrain;
    std::unique_ptr<SkyBox> m_skybox;
    std::shared_ptr<Train> m_playerTrain;
    std::shared_ptr<Train> m_enemyTrain;

    std::unique_ptr<LocalPlayer> m_localPlayer;
    std::unique_ptr<AIPlayer> m_aiPlayer;

    std::vector<AbstractGraphicsObject*> m_nodes;

    // bullet
    // these objects must not be deleted before m_bullet_dynamicsWorld
    // -- so as a temporary hack, we won't delete them at all
    btBroadphaseInterface * m_bulletBroadphase;
    btDefaultCollisionConfiguration * m_bulletCollisionConfig;
    btCollisionDispatcher * m_bulletDispatcher;
    btSequentialImpulseConstraintSolver * m_bulletSolver;
    btDiscreteDynamicsWorld * m_bulletWorld;
    std::unordered_map<const btCollisionObject*, AbstractPhysicsObject*> m_collisionMap;
};



}
