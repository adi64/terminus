#include "abstractwagon.h"

#include <QVector3D>
#include <QMatrix4x4>

#include <math.h>

#include <world/drawables/train/train.h>
#include <world/drawables/track.h>
#include <util/mathutil.h>

namespace terminus
{

AbstractWagon::AbstractWagon(World & world, Train * train)
    : KinematicPhysicsObject(world)
    , m_health(maxHealth())
    , m_disabled(false)
    , m_cooldown(0)
    , m_onCooldown(false)
    , m_train(train)
{
}

void AbstractWagon::primaryAction()
{

}

void AbstractWagon::primaryActionDebug()
{

}

void AbstractWagon::localUpdate()
{
    auto travelledDistance = m_train->travelledDistance() - m_positionOffset;

    QVector3D t = m_train->track()->tangentAt(travelledDistance);
    float angleY = 90.f + atan2(t.x(), t.z()) * 180.f / MathUtil::PI;
    KinematicPhysicsObject::setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.f, 1.f, 0.f), angleY));

    QVector3D trackOffset(0.f, 1.2f, 0.f);
    setPosition(m_train->track()->positionAt(travelledDistance) + trackOffset);
    KinematicPhysicsObject::localUpdate();
}

float AbstractWagon::maxHealth() const
{
    return 100.f;
}

bool AbstractWagon::isOnCooldown() const
{
    return m_onCooldown;
}

unsigned int AbstractWagon::cooldown() const
{
    return m_cooldown;
}

unsigned int AbstractWagon::maxCooldown() const
{
    return 100;
}

float AbstractWagon::currentHealth() const
{
    return m_health;
}

void AbstractWagon::setHealth(float health)
{
    if(health > maxHealth())
    {
        return;
    }

    if(health <= 0.0)
    {
        m_health = 0.0;
        m_disabled = true;
    }
    else
    {
        m_health = health;
        m_disabled = false;
    }
}

float AbstractWagon::length() const
{
    return 1.f;
}

bool AbstractWagon::isDisabled() const
{
    return m_disabled;
}

WagonType AbstractWagon::wagonType() const
{
    return INVALID;
}

void AbstractWagon::setPositionOffset(float accumulatedOffset)
{
    m_positionOffset = accumulatedOffset;
}

short AbstractWagon::myCollisionType() const
{
    return BulletWorld::CollisionTypes::COLLISIONTYPE_WAGON;
}

short AbstractWagon::possibleCollisionTypes() const
{
    return BulletWorld::CollisionTypes::COLLISIONTYPE_PROJECTILE;
}

}//namespace terminus
