#include "abstractwagon.h"

#include <QVector3D>
#include <QMatrix4x4>

#include <math.h>

#include <player/abstractplayer.h>
#include <util/mathutil.h>
#include <util/timer.h>
#include <world/drawables/projectile.h>
#include <world/drawables/train/train.h>
#include <world/drawables/track.h>

namespace terminus
{

AbstractWagon::AbstractWagon(World & world, Train * train)
: KinematicPhysicsObject(world)
, m_health(100.f)
, m_disabled(false)
, m_train(train)
{
    m_cooldownTimer = m_world.timer().allocateTimer();
    m_cameraTimer = m_world.timer().allocateTimer();
}

AbstractWagon::~AbstractWagon()
{
    m_world.timer().releaseTimer(m_cooldownTimer);
    m_world.timer().releaseTimer(m_cameraTimer);
}

void AbstractWagon::primaryAction()
{
    if(isDisabled() || isOnCooldown())
    {
        return;
    }

    primaryActionInternal();

    resetCooldown();
}

void AbstractWagon::primaryActionDebug()
{
    primaryActionInternal();
}

void AbstractWagon::localUpdate()
{
    auto travelledDistance = m_train->travelledDistance() - m_positionOffset;

    QVector3D t = m_train->track()->tangentAt(travelledDistance);
    float angleY = atan2(-t.z(), t.x()) * 180.f / MathUtil::PI;
    KinematicPhysicsObject::setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.f, 1.f, 0.f), angleY));

    QVector3D trackOffset(0.f, 0.f, 0.f);
    setPosition(m_train->track()->positionAt(travelledDistance) + trackOffset);
    KinematicPhysicsObject::localUpdate();
}

void AbstractWagon::onBindCamera()
{
    m_cameraEyeOffset = QVector3D(0.f, 0.f, 0.f);
    m_previousCenter = worldToModel(m_camera->center());
    m_previousEye = worldToModel(m_camera->eye());
    m_world.timer().adjust(m_cameraTimer, 0);
}

void AbstractWagon::adjustCamera()
{
    if(!m_camera)
    {
        return;
    }
    const int transitionTime = 200;
    float currentInfluence = MathUtil::linstep(0, transitionTime, m_world.timer().get(m_cameraTimer));

    auto vCenterM = MathUtil::mix(m_previousCenter, localCameraCenter(), currentInfluence);
    auto vEyeM = MathUtil::mix(m_previousEye, localCameraEye(), currentInfluence);

    m_camera->setCenter(modelToWorld(vCenterM));
    m_camera->setEye(modelToWorld(vEyeM));
}

void AbstractWagon::rotateEvent(QVector2D rotation)
{
    auto scale = 0.025f;
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

void AbstractWagon::resetCooldown() const
{
    m_world.timer().adjust(m_cooldownTimer, 0);
}

bool AbstractWagon::isOnCooldown() const
{
    return m_world.timer().get(m_cooldownTimer) < cooldownTime();
}

float AbstractWagon::cooldown() const
{
    return MathUtil::linstep(0.f, cooldownTime(), m_world.timer().get(m_cooldownTimer));
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
    return maxBB().x() - minBB().x();
}

bool AbstractWagon::isOtherTrainLeft() const
{
    return m_train->track()->isOtherTrackLeft();
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

void AbstractWagon::onCollisionWith(AbstractPhysicsObject *other)
{
    auto projectile = dynamic_cast<Projectile*>(other);
    if(projectile)
    {
        m_train->player().animateHit();
    }
}

short AbstractWagon::myCollisionType() const
{
    return BulletWorld::CollisionTypes::COLLISIONTYPE_WAGON;
}

short AbstractWagon::possibleCollisionTypes() const
{
    return BulletWorld::CollisionTypes::COLLISIONTYPE_PROJECTILE;
}

QVector3D AbstractWagon::localCameraCenter()
{
    auto & vBBMinM = minBB();
    auto & vBBMaxM = maxBB();

    auto xCenterM = (vBBMinM.x() + vBBMaxM.x()) * 0.5f;
    auto yBaseM = vBBMaxM.y() + 1.f;
    if(isOtherTrainLeft())
    {
        return {xCenterM, yBaseM, vBBMinM.z() - 1.f};
    }
    else
    {
        return {xCenterM, yBaseM, vBBMaxM.z() + 1.f};
    }
}

QVector3D AbstractWagon::localCameraEye()
{
    auto & vBBMinM = minBB();
    auto & vBBMaxM = maxBB();

    auto xCenterM = (vBBMinM.x() + vBBMaxM.x()) * 0.5f;
    auto yBaseM = vBBMaxM.y() + 1.f;
    auto & vEyeOff = m_cameraEyeOffset;
    if(isOtherTrainLeft())
    {
        return {xCenterM - vEyeOff.x(), yBaseM + vEyeOff.y(), vBBMaxM.z() + 2.f - vEyeOff.z()};
    }
    else
    {
        return {xCenterM + vEyeOff.x(), yBaseM + vEyeOff.y(), vBBMinM.z() - 2.f + vEyeOff.z()};
    }
}

}//namespace terminus
