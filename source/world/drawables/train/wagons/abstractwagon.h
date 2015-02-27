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

    virtual void primaryAction();
    virtual void primaryActionDebug();

    virtual void localUpdate() override;

    virtual void setHealth(float health);
    virtual float currentHealth() const;
    virtual float maxHealth() const;
    virtual bool isDisabled() const;
    virtual WagonType wagonType() const;
    virtual float length() const;

    virtual void setPositionOffset(float accumulatedOffset);

protected:
    float m_positionOffset;
    float m_health;
    bool m_disabled;
    Train *m_train;
};

}
