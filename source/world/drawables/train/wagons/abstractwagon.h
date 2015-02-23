#pragma once

#include <world/physics/kinematicphysicsobject.h>
#include <QQuaternion>

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

    virtual void adjustCamera() override;
    virtual void moveEvent(QVector3D movement);
    virtual void rotateEvent(QVector2D rotation);

    virtual float maxHealth() const;
    virtual float currentHealth() const;
    virtual void setHealth(float health);

    virtual float length() const;

    virtual float weight() const;
    virtual bool isDisabled() const;

    virtual void setPositionOffset(float accumulatedOffset);

protected:
    QQuaternion m_lockedEyeAngle;

    float m_positionOffset;
    float m_health;
    bool m_disabled;
    Train *m_train;
};

}
