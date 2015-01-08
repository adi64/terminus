#pragma once

#include <memory>

#include "abstractphysicsobject.h"

namespace terminus
{

class Projectile : public AbstractPhysicsObject
{
public:
    Projectile(Scene *scene);

    void update(int elapsedMilliseconds) override;
    void render(QOpenGLFunctions& gl) const override;

protected:
    virtual unsigned int maxAgeInMilliseconds() const;
    unsigned int m_ageInMilliseconds;
};

}
