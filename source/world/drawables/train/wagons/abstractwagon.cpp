#include "abstractwagon.h"

#include <QVector3D>
#include <QMatrix4x4>

#include <math.h>

#include <world/drawables/train/train.h>
#include <world/drawables/track.h>

#include <util/mathutil.h>
#include <util/timer.h>

namespace terminus
{

AbstractWagon::AbstractWagon(World & world, Train * train)
: KinematicPhysicsObject(world)
, m_health(maxHealth())
, m_disabled(false)
, m_cooldown(1.0f)
, m_onCooldown(false)
, m_train(train)
{
}

void AbstractWagon::primaryActionDebug()
{

}

void AbstractWagon::localUpdate()
{
    Timer::TimerMSec frameDuration = m_world.timer().get("frameTimer");
    if(m_onCooldown)
    {
        m_cooldown += (frameDuration / cooldownRate());
        if(m_cooldown >= 1.f)
        {
            m_cooldown = 1.f;
            m_onCooldown = false;
        }
    }

    auto travelledDistance = m_train->travelledDistance() - m_positionOffset;

    QVector3D t = m_train->track()->tangentAt(travelledDistance);
    float angleY = 90.f + atan2(t.x(), t.z()) * 180.f / MathUtil::PI;
    KinematicPhysicsObject::setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.f, 1.f, 0.f), angleY));

    QVector3D trackOffset(0.f, 1.2f, 0.f);
    setPosition(m_train->track()->positionAt(travelledDistance) + trackOffset);
    KinematicPhysicsObject::localUpdate();
}

void AbstractWagon::onBindCamera()
{
    m_cameraEyeOffset = QVector3D(0.f, 0.f, 0.f);
}

void AbstractWagon::adjustCamera()
{
    if(!m_camera)
    {
        return;
    }
    Camera & camera = *m_camera;

    auto & vBBMinM = minBB();
    auto & vBBMaxM = maxBB();
    auto vCenterM = QVector3D(0.f, vBBMaxM.y() + 1.f, vBBMaxM.z());
    auto vEyeM = QVector3D(0.f, vBBMaxM.y() + 1.f, vBBMinM.z() - 2.f) + m_cameraEyeOffset;

    auto vCenterW4 = modelMatrix() * QVector4D(vCenterM, 1.f);
    auto vEyeW4 = modelMatrix() * QVector4D(vEyeM, 1.f);

    camera.setCenter(vCenterW4.toVector3DAffine());
    camera.setEye(vEyeW4.toVector3DAffine());
}

void AbstractWagon::moveEvent(QVector3D /*movement*/)
{
}

void AbstractWagon::rotateEvent(QVector2D rotation)
{
    auto scale = 0.025;
    auto & vBBMinM = minBB();
    auto & vBBMaxM = maxBB();
    m_cameraEyeOffset.setX(
        MathUtil::clamp(vBBMinM.x(), vBBMaxM.x(), m_cameraEyeOffset.x() + rotation.x() * scale));
    m_cameraEyeOffset.setY(
        MathUtil::clamp(-1.f, 3.f, m_cameraEyeOffset.y() + rotation.y() * scale));
}

float AbstractWagon::maxHealth() const
{
    return 100.f;
}

bool AbstractWagon::isOnCooldown() const
{
    return m_onCooldown;
}

float AbstractWagon::cooldown() const
{
    return m_cooldown;
}

float AbstractWagon::cooldownRate() const
{
    return 1.f;
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
