#include "bulletworld.h"

#include <QDebug>

#include <world/physics/abstractphysicsobject.h>

namespace terminus
{

void BulletWorld::btStaticTickCallback(btDynamicsWorld *world, btScalar timeStep)
{
    // retrieve instance pointer from user info (void*)
    auto gameInstance = static_cast<BulletWorld*>(world->getWorldUserInfo());
    gameInstance->btTickCallback(world, timeStep);
}

BulletWorld::BulletWorld()
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
    m_bulletWorld->setInternalTickCallback(&BulletWorld::btStaticTickCallback);
    m_bulletWorld->setWorldUserInfo(static_cast<void*>(this));
}

BulletWorld::~BulletWorld()
{
    delete m_bulletWorld;
    delete m_bulletSolver;
    delete m_bulletBroadphase;
    delete m_bulletDispatcher;
    delete m_bulletCollisionConfig;
}

void BulletWorld::addRigidBody(btRigidBody *rigidBody)
{
    m_bulletWorld->addRigidBody(rigidBody);
}

void BulletWorld::removeRigidBody(btRigidBody *rigidBody)
{
    m_bulletWorld->removeRigidBody(rigidBody);
}

int BulletWorld::stepSimulation(btScalar timeStep, int maxSubSteps, btScalar fixedTimeStep)
{
   return m_bulletWorld->stepSimulation(timeStep, maxSubSteps, fixedTimeStep);
}

void BulletWorld::addCollisionMapping(const btCollisionObject *collisionObject, AbstractPhysicsObject *graphicsObject)
{
    auto newPair = std::pair<const btCollisionObject*, AbstractPhysicsObject*>(collisionObject, graphicsObject);
    m_collisionMap.insert(newPair);
}

void BulletWorld::removeCollisionMapping(const btCollisionObject *collisionObject)
{
    m_collisionMap.erase(collisionObject);
}

AbstractPhysicsObject *BulletWorld::getPhysicsObjectForCollisionObject(const btCollisionObject *collisionObject) const
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

void BulletWorld::btTickCallback(btDynamicsWorld *world, btScalar)
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
            auto physicsObject0 = getPhysicsObjectForCollisionObject(body0);
            auto physicsObject1 = getPhysicsObjectForCollisionObject(body1);

            if(physicsObject0 != nullptr && physicsObject1 != nullptr)
            {
                physicsObject0->onCollisionWith(physicsObject1);
                physicsObject1->onCollisionWith(physicsObject0);
            }
        }
    }
}


} // namespace terminus
