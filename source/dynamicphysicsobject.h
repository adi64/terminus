#pragma once

#include <bullet/btBulletDynamicsCommon.h>

#include "abstractphysicsobject.h"

namespace terminus
{

class Scene;

class DynamicPhysicsObject : public AbstractPhysicsObject
{
public:
    DynamicPhysicsObject(Scene* scene);

    virtual void update(int elapsedMilliseconds) override;

    void applyForce(const QVector3D &force);
};

}
