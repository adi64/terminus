#pragma once

#include <QString>
#include <memory>

#include <world/physics/kinematicphysicsobject.h>

namespace terminus
{
class Train;

enum WagonType
{
    INVALID = 0,
    ENGINE_WAGON = 1,
    WEAPON_WAGON = 2,
    REPAIR_WAGON = 3
};

class AbstractWagon : public KinematicPhysicsObject
{
public:
    AbstractWagon(World & world, Train * train);

    virtual void primaryAction() = 0;
    virtual void primaryActionDebug();

    virtual void localUpdate() override;

    virtual void adjustCamera() override;

    virtual void setHealth(float health);
    virtual float currentHealth() const;
    virtual float maxHealth() const;

    virtual float cooldown() const;
    virtual float cooldownRate() const = 0;
    virtual bool isOnCooldown() const;

    virtual bool isDisabled() const;
    virtual WagonType wagonType() const;
    virtual float length() const;

    virtual void setPositionOffset(float accumulatedOffset);

protected:
    virtual short myCollisionType() const override;
    virtual short possibleCollisionTypes() const override;

protected:
    float m_positionOffset;
    float m_health;
    bool m_disabled;
    float m_cooldown;
    bool m_onCooldown;
    Train *m_train;
};

}
