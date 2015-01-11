#pragma once

#include <bullet/btBulletDynamicsCommon.h>

#include "abstractphysicsobject.h"

namespace terminus
{

class DynamicPhysicsObject : public AbstractPhysicsObject
{
public:
    DynamicPhysicsObject(const std::shared_ptr<Scene> &scene);

    virtual void update(int elapsedMilliseconds) override;

    void applyForce(const QVector3D &force);
};

}
