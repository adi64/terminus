#pragma once

#include <QString>
#include <memory>

#include <util/timer.h>
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
    virtual ~AbstractWagon();

    virtual void primaryAction() = 0;
    virtual void primaryActionDebug();

    virtual void localUpdate() override;

    virtual void onBindCamera() override;
    virtual void adjustCamera();
    virtual void moveEvent(QVector3D movement);
    virtual void rotateEvent(QVector2D rotation);

    virtual void setHealth(float health);
    virtual float currentHealth() const;
    virtual float maxHealth() const;

    virtual void resetCooldown() const;
    virtual float cooldown() const;
    virtual float cooldownTime() const = 0;
    virtual bool isOnCooldown() const;

    virtual bool isDisabled() const;
    virtual WagonType wagonType() const;
    virtual float length() const;

    virtual float isOtherTrainLeft() const;

    virtual void setPositionOffset(float accumulatedOffset);

    virtual void onCollisionWith(AbstractPhysicsObject * other) override;

protected:
    virtual short myCollisionType() const override;
    virtual short possibleCollisionTypes() const override;

protected:

    QVector3D m_cameraEyeOffset;

    float m_positionOffset;
    float m_health;
    bool m_disabled;

    Timer::TimerID m_cooldownTimer;

    Train *m_train;
};

}
