#pragma once

#include <memory>

#include <util/timer.h>
#include <world/physics/dynamicphysicsobject.h>

namespace terminus
{

class Projectile : public DynamicPhysicsObject
{
public:
    Projectile(World & world);
    virtual ~Projectile();

    virtual void localUpdate() override;

    virtual void localRenderSetup(QOpenGLFunctions & gl, Program & program) const override;

    virtual float damage() const;
    virtual void onCollisionWith(AbstractPhysicsObject* other) override;

protected:
    virtual unsigned int maxAgeInMilliseconds() const;

protected:
    Timer::TimerID m_lifeTimer;
};

}
