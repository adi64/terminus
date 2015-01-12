#pragma once

#include <bullet/btBulletDynamicsCommon.h>

#include "abstractphysicsobject.h"

namespace terminus
{

class KinematicPhysicsObject : public AbstractPhysicsObject
{
public:
    KinematicPhysicsObject(const std::shared_ptr<Scene> &scene);
};

}
