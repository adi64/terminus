#pragma once

#include <QString>
#include <memory>

#include <world/physics/kinematicphysicsobject.h>

#include "qmlwagon.h"

namespace terminus
{
class Train;

class AbstractWagon : public KinematicPhysicsObject
{

public:
    AbstractWagon(std::shared_ptr<Scene> scene, Train *train);

    virtual void primaryAction();
    virtual void primaryActionDebug();

    virtual void update(int elapsedMilliseconds) override;

    virtual QMLWagon *qmlWagon() const final;

    virtual void setHealth(float health);
    virtual float currentHealth() const;
    virtual float maxHealth() const;
    virtual bool isDisabled() const;
    virtual QString wagonType() const = 0;
    virtual float length() const;

    virtual void setPositionOffset(float accumulatedOffset);

protected:
    std::unique_ptr<QMLWagon> m_qmlWagon;
    float m_positionOffset;
    float m_health;
    bool m_disabled;
    Train *m_train;
};

}
