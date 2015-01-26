#pragma once

#include "../kinematicphysicsobject.h"

namespace terminus
{
class Train;

class AbstractWagon : public KinematicPhysicsObject
{
public:
    AbstractWagon(std::shared_ptr<Scene> scene, Train *train);

    virtual void primaryAction();

    virtual void update(int elapsedMilliseconds) override;

    virtual float maxHealth() const;
    virtual float currentHealth();
    virtual void setHealth(float health);

    virtual float length() const;
    virtual void setPositionOffset(float accumulatedOffset);

protected:
    float m_positionOffset;
    float m_health;
    Train *m_train;
};

}
