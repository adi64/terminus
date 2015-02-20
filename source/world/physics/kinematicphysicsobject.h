#pragma once

#include "abstractphysicsobject.h"

namespace terminus
{

class KinematicPhysicsObject : public AbstractPhysicsObject
{
public:
    KinematicPhysicsObject(std::shared_ptr<Scene> scene);

    virtual void localUpdate(int elapsedMilliseconds) override;

protected:
    virtual void initializePhysics(btCollisionShape * collisionShape, btScalar mass) override;
};

}
