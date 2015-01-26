#pragma once

#include <memory>

#include "dynamicphysicsobject.h"

namespace terminus
{

class Projectile : public DynamicPhysicsObject
{
public:
    Projectile(std::shared_ptr<Scene> scene);
    virtual ~Projectile();

    virtual void update(int elapsedMilliseconds) override;

    virtual void preRender(QOpenGLFunctions & gl, Program & program) const override;

protected:
    virtual unsigned int maxAgeInMilliseconds() const;

protected:
    unsigned int m_ageInMilliseconds;
};

}
