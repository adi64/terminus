#pragma once

#include <unordered_map>

#include <bullet/btBulletDynamicsCommon.h>

namespace terminus
{

class AbstractPhysicsObject;

/*!
 * \brief A Wrapper class for a btDiscreteDynamicsWorld
 */
class BulletWorld
{
protected:
    static void btStaticTickCallback(btDynamicsWorld * world, btScalar timeStep);

public:
    BulletWorld();
    ~BulletWorld();

    BulletWorld(const BulletWorld &other) = delete;
    BulletWorld &operator=(const BulletWorld &other) = delete;

    /*!
     * \brief Wraps btDiscreteDynamicsWorld::addRigidBody()
     */
    void addRigidBody(btRigidBody* rigidBody);

    /*!
     * \brief Wraps btDiscreteDynamicsWorld::removeRigidBody()
     */
    void removeRigidBody(btRigidBody* rigidBody);

    /*!
     * \brief Wraps btDiscreteDynamicsWorld::stepSimulation()
     */
    int	stepSimulation(btScalar timeStep, int maxSubSteps = 1, btScalar fixedTimeStep = btScalar(1.)/btScalar(60.));

    void addCollisionMapping(const btCollisionObject * collisionObject, AbstractPhysicsObject * graphicsObject);
    void removeCollisionMapping(const btCollisionObject * collisionObject);
    AbstractPhysicsObject * getPhysicsObjectForCollisionObject(const btCollisionObject * collisionObject) const;

protected:
    void btTickCallback(btDynamicsWorld * world, btScalar timeStep);

    btBroadphaseInterface * m_bulletBroadphase;
    btDefaultCollisionConfiguration * m_bulletCollisionConfig;
    btCollisionDispatcher * m_bulletDispatcher;
    btSequentialImpulseConstraintSolver * m_bulletSolver;
    btDiscreteDynamicsWorld * m_bulletWorld;
    std::unordered_map<const btCollisionObject*, AbstractPhysicsObject*> m_collisionMap;
};

} // namespace terminus
