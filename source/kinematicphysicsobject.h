#pragma once

#include <bullet/btBulletDynamicsCommon.h>

#include "abstractphysicsobject.h"

namespace terminus
{

class Scene;

class KinematicPhysicsObject : public AbstractPhysicsObject
{
public:
    KinematicPhysicsObject(Scene* scene);

    virtual void update(int elapsedMilliseconds) override;

    virtual void setPosition(const QVector3D& newPosition) override;
};

}
