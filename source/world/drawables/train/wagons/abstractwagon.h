#pragma once

#include <world/physics/kinematicphysicsobject.h>

namespace terminus
{
class Train;

class AbstractWagon : public KinematicPhysicsObject
{
public:
    AbstractWagon(std::shared_ptr<Scene> scene, Train *train);

    virtual void primaryAction();
    virtual void primaryActionDebug();

    virtual void localUpdate(int elapsedMilliseconds) override;

    virtual float maxHealth() const;
    virtual float currentHealth() const;
    virtual void setHealth(float health);

    virtual float length() const;

    virtual float weight() const;
    virtual bool isDisabled() const;

    virtual void setPositionOffset(float accumulatedOffset);

protected:
    float m_positionOffset;
    float m_health;
    bool m_disabled;
    Train *m_train;
};

}
