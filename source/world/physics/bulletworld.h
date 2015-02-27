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
    /*!
     * \brief Static wrapper for BulletWorld::btTickCallback()
     *
     * Since bullet can only store plain C function pointers, this static method will be called on every tick (substep).
     * A bullet dynamics world has a user-definable void* storage that we can use to determine the BulletWorld instance which should receive the call.
     *
     * \sa btTickCallback()
     */
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
    /*!
     * \brief A Callback that is called on every simulation tick
     *
     * This method gets called on every simulation tick (substep). At this point, we can check for colliding objects and dispatch collision events.
     */
    void btTickCallback(btDynamicsWorld * world, btScalar timeStep);

    btBroadphaseInterface * m_bulletBroadphase;
    btDefaultCollisionConfiguration * m_bulletCollisionConfig;
    btCollisionDispatcher * m_bulletDispatcher;
    btSequentialImpulseConstraintSolver * m_bulletSolver;
    btDiscreteDynamicsWorld * m_bulletWorld;
    std::unordered_map<const btCollisionObject*, AbstractPhysicsObject*> m_collisionMap;
};

} // namespace terminus
