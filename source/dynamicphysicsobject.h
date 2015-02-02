#pragma once

#include "abstractphysicsobject.h"

namespace terminus
{

class DynamicPhysicsObject : public AbstractPhysicsObject
{
public:
    DynamicPhysicsObject(std::shared_ptr<Scene> scene);

    virtual void update() override;

    virtual void applyForce(const QVector3D & force);
};

}
