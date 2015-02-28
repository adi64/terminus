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
