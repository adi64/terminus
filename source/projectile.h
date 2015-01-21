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
    void render(QOpenGLFunctions& gl) const override;

protected:
    virtual unsigned int maxAgeInMilliseconds() const;
    unsigned int m_ageInMilliseconds;
};

}
