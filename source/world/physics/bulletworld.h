#pragma once

#include <unordered_map>

#include <bullet/btBulletDynamicsCommon.h>

namespace terminus
{

class AbstractPhysicsObject;

/*!
 * \brief A Wrapper class for a btDiscreteDynamicsWorld
 *
 * Additionally contains helper functions and a map from btCollisionObject to AbstractPhysicsObject
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
     *
     * Note that rigid bodies added via this method will collide with any other object in the scene.
     */
    void addRigidBody(btRigidBody* rigidBody);

    /*!
     * \brief Wraps btDiscreteDynamicsWorld::addRigidBody()
     *
     * Rigid bodies added via this method have a collision group and will only collide with objects that match the given collision mask.
     *
     * \sa AbstractPhysicsObject::myCollisionType() and AbstractPhysicsObject::possibleCollisionTypes()
     */
    void addRigidBody(btRigidBody* rigidBody, short group, short mask);

    /*!
     * \brief Wraps btDiscreteDynamicsWorld::removeRigidBody()
     */
    void removeRigidBody(btRigidBody* rigidBody);

    /*!
     * \brief Wraps btDiscreteDynamicsWorld::stepSimulation()
     */
    int	stepSimulation(btScalar timeStep, int maxSubSteps = 1, btScalar fixedTimeStep = btScalar(1.)/btScalar(60.));

    /*!
     * \brief Add mapping from btCollisionObject to AbstractPhysicsObject
     * to m_collisionMap
     * \param collisionObject A pointer to a btCollisionObject (referenced in
     * btRigidBody of AbstractPhysicsObject)
     * \param graphicsObject A pointer to the corresponding AbstractPhysicsObject
     */
    void addCollisionMapping(const btCollisionObject * collisionObject, AbstractPhysicsObject * graphicsObject);

    /*!
     * \brief Remove mapping from m_collisionMap
     * \param collisionObject A pointer to a btCollisionObject
     */
    void removeCollisionMapping(const btCollisionObject * collisionObject);
    AbstractPhysicsObject * getPhysicsObjectForCollisionObject(const btCollisionObject * collisionObject) const;

    /*!
     * \brief Bitfield values that describe the groups of objects that can collide with each other
     *
     * \sa AbstractPhysicsObject::myCollisionType() and AbstractPhysicsObject::possibleCollisionTypes()
     */
    enum CollisionTypes
    {
        COLLISIONTYPE_NOTHING       = 0,
        COLLISIONTYPE_WAGON         = 1 << 0,
        COLLISIONTYPE_PROJECTILE    = 1 << 1,
        COLLISIONTYPE_TERRAIN       = 1 << 2
    };

protected:
    /*!
     * \brief A Callback that is called on every simulation tick
     *
     * This method gets called on every simulation tick (substep).
     * At this point, we can check for colliding objects and dispatch collision events.
     */
    void btTickCallback(btDynamicsWorld * world, btScalar timeStep);

    btBroadphaseInterface * m_bulletBroadphase;
    btDefaultCollisionConfiguration * m_bulletCollisionConfig;
    btCollisionDispatcher * m_bulletDispatcher;
    btSequentialImpulseConstraintSolver * m_bulletSolver;
    btDiscreteDynamicsWorld * m_bulletWorld;

    /*!
     * \brief This map is used to get the AbstractPhysicsObject belonging to a
     * btCollisionObject.
     *
     * \sa btTickCallback
     */
    std::unordered_map<const btCollisionObject*, AbstractPhysicsObject*> m_collisionMap;
};

} // namespace terminus
