#pragma once

#include "abstractphysicsobject.h"

namespace terminus
{

class DynamicPhysicsObject : public AbstractPhysicsObject
{
public:
    DynamicPhysicsObject(std::shared_ptr<Scene> scene);

    virtual void update(int elapsedMilliseconds) override;

    virtual void applyForce(const QVector3D & force);
    virtual void setLinearVelocity(const QVector3D & velocity);
};

}
