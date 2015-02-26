#pragma once

#include <QString>
#include <memory>

#include <world/physics/kinematicphysicsobject.h>

namespace terminus
{
class Train;

enum WagonType
{
    ENGINE_WAGON = 0,
    WEAPON_WAGON = 1,
    REPAIR_WAGON = 2
};

class AbstractWagon : public KinematicPhysicsObject
{

public:
    AbstractWagon(std::shared_ptr<Scene> scene, Train *train);

    virtual void primaryAction();
    virtual void primaryActionDebug();

    virtual void update(int elapsedMilliseconds) override;

    virtual void setHealth(float health);
    virtual float currentHealth() const;
    virtual float maxHealth() const;
    virtual bool isDisabled() const;
    virtual WagonType wagonType() const = 0;
    virtual float length() const;

    virtual void setPositionOffset(float accumulatedOffset);

protected:
    float m_positionOffset;
    float m_health;
    bool m_disabled;
    Train *m_train;
};

}
