#pragma once

#include <memory>

#include <bullet/btBulletDynamicsCommon.h>

#include <world/drawables/abstractgraphicsobject.h>

namespace terminus
{

class AbstractPhysicsObject : public AbstractGraphicsObject
{    
public:
    AbstractPhysicsObject(World & world);

    virtual void moveTo(const QVector3D & newPosition);

    /*!
     * \brief Gets called on collision with another AbstractPhysicsObject
     * \param other AbstractPhysicsObject that we collided with
     *
     * This default implementation does nothing and only exists so that subclasses can override it with custom functionality (like dealing damage).
     * Movement changes are handled by bullet.
     */
    virtual void onCollisionWith(AbstractPhysicsObject* other);

    /*!
     * \brief Set whether this PhysicsObject originated in this local game instance or in a remote instance
     * \param spawnedLocally
     */
    virtual void setSpawnedLocally(bool spawnedLocally);

protected:
    virtual void initializePhysics(btCollisionShape * collisionShape, btScalar mass);
    virtual void deallocatePhysics();

protected:
    std::unique_ptr<btRigidBody> m_btRigidBody;
    bool m_spawnedLocally;
};

}
