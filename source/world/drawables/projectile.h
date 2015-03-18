#pragma once

#include <util/timer.h>
#include <world/physics/dynamicphysicsobject.h>

namespace terminus
{

/*!
 * \brief The Projectile class simulates and renders a projectile.
 * It also handles collisions with Wagons, deals damage and spawns
 * Explosions.
 *
 * Projectiles have a limited lifetime and disappear if they do not hit
 * a target within that time.
 */
class Projectile : public DynamicPhysicsObject
{
public:
    Projectile(World & world);
    virtual ~Projectile();

    virtual void localUpdate() override;

    virtual void onCollisionWith(AbstractPhysicsObject* other) override;

protected:
    float damage() const;
    int maxAgeInMilliseconds() const;

    virtual short myCollisionType() const override;
    virtual short possibleCollisionTypes() const override;

protected:
    Timer::TimerID m_lifeTimer;
};

}
