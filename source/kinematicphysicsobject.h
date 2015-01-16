#pragma once

#include <bullet/btBulletDynamicsCommon.h>

#include "abstractphysicsobject.h"

namespace terminus
{

class KinematicPhysicsObject : public AbstractPhysicsObject
{
public:
    KinematicPhysicsObject(std::shared_ptr<Scene> scene);

    virtual void update(int elapsedMilliseconds) override;
};

}
