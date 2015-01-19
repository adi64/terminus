#pragma once

#include <memory>

#include "dynamicphysicsobject.h"

namespace terminus
{

class Projectile : public DynamicPhysicsObject
{
public:
    Projectile(std::shared_ptr<Scene> scene);

    void update(int elapsedMilliseconds) override;

    virtual void preRender(QOpenGLFunctions & gl, Program & program) const override;

protected:
    virtual unsigned int maxAgeInMilliseconds() const;
    unsigned int m_ageInMilliseconds;
};

}
