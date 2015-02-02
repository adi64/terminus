#pragma once

#include <memory>

#include "dynamicphysicsobject.h"
#include "timer.h"

namespace terminus
{

class Projectile : public DynamicPhysicsObject
{
public:
    Projectile(std::shared_ptr<Scene> scene);
    virtual ~Projectile();

    virtual void update() override;

    virtual void preRender(QOpenGLFunctions & gl, Program & program) const override;

    virtual float damage() const;

protected:
    virtual unsigned int maxAgeInMilliseconds() const;

protected:
    Timer::TimerID m_age;
};

}
