#pragma once

#include <world/physics/abstractphysicsobject.h>

namespace terminus
{

class KinematicPhysicsObject : public AbstractPhysicsObject
{
public:
    KinematicPhysicsObject(World & world);

    virtual void localUpdate() override;

protected:
    virtual void initializePhysics(btCollisionShape * collisionShape, btScalar mass) override;
};

}
