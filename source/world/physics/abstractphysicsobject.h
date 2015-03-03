#pragma once

#include <memory>

#include <world/drawables/abstractgraphicsobject.h>
#include <world/physics/bulletworld.h>

namespace terminus
{

class AbstractPhysicsObject : public AbstractGraphicsObject
{    
public:
    AbstractPhysicsObject(World & world);

    virtual void moveTo(const QVector3D & newPosition);
    virtual void onCollisionWith(AbstractPhysicsObject* other);

protected:
    virtual void initializePhysics(btCollisionShape * collisionShape, btScalar mass);
    virtual void deallocatePhysics();

    /*!
     * \brief The collision group that this object is member of
     * \return a bitfield that defines the collision group.
     * \sa BulletWorld::CollisionTypes
     */
    virtual short myCollisionType() const = 0;

    /*!
     * \brief The groups of objects that this object will collide with
     * \return a bitfield that defines with with groups this object will collide
     * \sa BulletWorld::CollisionTypes
     */
    virtual short possibleCollisionTypes() const = 0;

protected:
    /*!
     * \brief Shared pointer to the bullet world that this object lives in
     *
     * Every physics object holds a shared pointer to the BulletWorld it lives in
     * in order to extend the BulletWorld's lifetime to the point where this physics object dies
     */
    std::shared_ptr<BulletWorld> m_bulletWorld;
    std::unique_ptr<btRigidBody> m_btRigidBody;
};

}
