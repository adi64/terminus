#pragma once

#include <memory>

#include <world/physics/dynamicphysicsobject.h>

namespace terminus
{

class Projectile : public DynamicPhysicsObject
{
public:
    Projectile(std::shared_ptr<Scene> scene);
    virtual ~Projectile();

    virtual void localUpdate(int elapsedMilliseconds) override;

    virtual void localRenderSetup(QOpenGLFunctions & gl, Program & program) const override;

    virtual float damage() const;
    virtual void onCollisionWith(AbstractPhysicsObject* other) override;

protected:
    virtual unsigned int maxAgeInMilliseconds() const;

protected:
    unsigned int m_ageInMilliseconds;
};

}
