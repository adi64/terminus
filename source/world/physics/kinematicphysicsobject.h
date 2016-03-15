#pragma once

#include "abstractphysicsobject.h"

namespace terminus
{
class AbstractGraphicsObject;

/*!
 * \brief A kinematic physics object is a "scripted" physics object, meaning
 * that it will set its own movement. It will be used in collision detection,
 * but bullet will not simulate its movement.
 */
class KinematicPhysicsObject : public AbstractPhysicsObject
{
public:
    explicit KinematicPhysicsObject(World & world, AbstractGraphicsObject * parent = nullptr);

    virtual void localUpdate() override;

protected:
    virtual void initializePhysics(btCollisionShape * collisionShape, btScalar mass) override;
};

}
