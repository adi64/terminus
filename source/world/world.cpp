#include "world.h"

#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QTime>

#include <bullet/btBulletDynamicsCommon.h>


#include <game.h>
#include <world/drawables/train/train.h>
#include <world/drawables/terrain.h>
#include <world/drawables/skybox.h>
#include <world/drawables/projectile.h>
#include <world/drawables/train/wagons/enginewagon.h>
#include <world/drawables/train/wagons/weaponwagon.h>
#include <world/drawables/train/wagons/repairwagon.h>

#include <player/abstractplayer.h>
#include <player/aiplayer.h>
#include <player/localplayer.h>

#include <world/camera.h>
#include <world/physics/abstractphysicsobject.h>
#include <deferredactionhandler.h>

namespace terminus
{

void World::btStaticTickCallback(btDynamicsWorld * world, btScalar timeStep)
{
    // retrieve instance pointer from user info (void*)
    auto gameInstance = static_cast<World*>(world->getWorldUserInfo());
    gameInstance->btTickCallback(world, timeStep);
}

World::World(Game & game)
: m_game(game)
{
    // bullets needs to be initialized before the other members
    setupBullet();

    m_terrain = std::unique_ptr<Terrain>(new Terrain(*this));
    m_skybox = std::unique_ptr<SkyBox>(new SkyBox(*this));

    m_playerTrain = std::shared_ptr<Train>(new Train(*this, m_terrain->playerTrack()));
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<RepairWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<RepairWagon>();
    m_playerTrain->addWagon<WeaponWagon>();

    m_enemyTrain = std::shared_ptr<Train>(new Train(*this, m_terrain->enemyTrack()));
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<RepairWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<RepairWagon>();

    m_enemyTrain->follow(m_playerTrain);

    m_localPlayer = std::unique_ptr<LocalPlayer>(new LocalPlayer(m_playerTrain));
    m_aiPlayer = std::unique_ptr<AIPlayer>(new AIPlayer(m_enemyTrain, m_playerTrain));

    addNode(m_playerTrain.get());
    addNode(m_enemyTrain.get());
    addNode(m_terrain.get());
    addNode(m_skybox.get());

    localPlayer().camera().setEye(QVector3D(-30.0, 10.0, 20.0));
    localPlayer().camera().setCenter(QVector3D(0.0, 0.0, 10.0));
    localPlayer().camera().setUp(QVector3D(0.0, 1.0, 0.0));
    localPlayer().camera().lockToObject(m_playerTrain->wagonAt(0));
}

World::~World()
{
    //we will not delete bullet pointers as that would cause segfaults if the application terminates
    //TODO fix it by using a shared bullet object pointer
    //deleteBullet();
}

void World::addNode(AbstractGraphicsObject *node)
{
    m_nodes.push_back(node);
}

void World::deleteNode(AbstractGraphicsObject *node)
{
    for(auto iterator = m_nodes.begin(); iterator != m_nodes.end(); ++iterator)
    {
        if(*iterator == node)
        {
            m_nodes.erase(iterator);
            return;
        }
    }

    qDebug() << "Could not find node " << node;
}

void World::addCollisionMapping(const btCollisionObject *collisionObject, AbstractPhysicsObject *graphicsObject)
{
    auto newPair = std::pair<const btCollisionObject*, AbstractPhysicsObject*>(collisionObject, graphicsObject);
    m_collisionMap.insert(newPair);
}

void World::removeCollisionMapping(const btCollisionObject *collisionObject)
{
    m_collisionMap.erase(collisionObject);
}

AbstractPhysicsObject *World::getGraphicsObjectForCollisionObject(const btCollisionObject *collisionObject) const
{
    try
    {
        return m_collisionMap.at(collisionObject);
    }
    catch(std::exception e)
    {
        qDebug() << "could not find AbstractGraphicsObject for collisionObject " << collisionObject;
        return nullptr;
    }
}

void World::setupBullet()
{
    m_bulletCollisionConfig = new btDefaultCollisionConfiguration();
    m_bulletDispatcher = new btCollisionDispatcher(m_bulletCollisionConfig);
    m_bulletBroadphase = new btDbvtBroadphase();
    m_bulletSolver = new btSequentialImpulseConstraintSolver;

    m_bulletWorld = new btDiscreteDynamicsWorld(
                                        m_bulletDispatcher,
                                        m_bulletBroadphase,
                                        m_bulletSolver,
                                        m_bulletCollisionConfig);
    m_bulletWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
    // set world user info (void*) to pointer to this game instance
    // so we can (indirectly) call a member of Game without having global state or a singleton
    m_bulletWorld->setInternalTickCallback(&World::btStaticTickCallback);
    m_bulletWorld->setWorldUserInfo(static_cast<void*>(this));
}

void World::deleteBullet()
{
    delete m_bulletWorld;
    delete m_bulletSolver;
    delete m_bulletBroadphase;
    delete m_bulletDispatcher;
    delete m_bulletCollisionConfig;
}

void World::btTickCallback(btDynamicsWorld *world, btScalar timeStep)
{
    int numManifolds = world->getDispatcher()->getNumManifolds();

    for (int i=0; i < numManifolds; ++i)
    {
        auto contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
        auto body0 = contactManifold->getBody0();
        auto body1 = contactManifold->getBody1();

        auto numContacts = contactManifold->getNumContacts();

        if(numContacts > 0)
        {
            auto physicsObject0 = getGraphicsObjectForCollisionObject(body0);
            auto physicsObject1 = getGraphicsObjectForCollisionObject(body1);

            if(physicsObject0 != nullptr && physicsObject1 != nullptr)
            {
                physicsObject0->onCollisionWith(physicsObject1);
                physicsObject1->onCollisionWith(physicsObject0);
            }
        }
    }
}

void World::update(int elapsedMilliseconds)
{
    // physics
    m_bulletWorld->stepSimulation((float)elapsedMilliseconds / 1000.0f, 10);

    for(auto node : m_nodes)
    {
        node->update(elapsedMilliseconds);
    }


    m_aiPlayer->update(elapsedMilliseconds);
    m_localPlayer->update(elapsedMilliseconds);
    // camera updates after all other nodes because it can follow the position of other nodes
    m_localPlayer->camera().update();
}

void World::render(QOpenGLFunctions & gl) const
{
    gl.glViewport(0, 0, m_localPlayer->camera().viewport().x(), m_localPlayer->camera().viewport().y());

    gl.glClearColor(0.5f, 0.55f, 0.6f, 1.0f);
    gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gl.glEnable(GL_BLEND);
    gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gl.glEnable(GL_CULL_FACE);
    gl.glFrontFace(GL_CCW); //TODO check if still necessary after geometry loader

    gl.glEnable(GL_DEPTH_TEST);
    gl.glDepthMask(GL_TRUE);
    gl.glDepthFunc(GL_LESS);

    for(auto node : m_nodes)
    {
        node->render(gl);
    }

    gl.glDisable(GL_BLEND);
    gl.glDisable(GL_DEPTH_TEST);
    gl.glDisable(GL_CULL_FACE);

}

LocalPlayer & World::localPlayer()
{
    return *m_localPlayer;
}

Train & World::playerTrain()
{
    return *m_playerTrain;
}

Train & World::enemyTrain()
{
    return *m_enemyTrain;
}

btDiscreteDynamicsWorld * World::bulletWorld()
{
    return m_bulletWorld;
}

void World::scheduleAction(DeferredAction event)
{
    m_game.deferredActionHandler().scheduleAction(event);
}

} // namespace terminus
