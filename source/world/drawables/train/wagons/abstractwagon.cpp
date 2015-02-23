#include "abstractwagon.h"

#include <QVector3D>
#include <QMatrix4x4>

#include <math.h>

#include <world/drawables/train/train.h>
#include <world/drawables/track.h>
#include <util/mathutil.h>

namespace terminus
{

constexpr QVector3D m_lockedCenterOffset = QVector3D(0.0, 2.5, 0.0);

AbstractWagon::AbstractWagon(std::shared_ptr<Scene> scene, Train *train)
    : KinematicPhysicsObject(scene)
    , m_health(maxHealth())
    , m_disabled(false)
    , m_train(train)
{
}

void AbstractWagon::primaryAction()
{

}

void AbstractWagon::primaryActionDebug()
{

}

void AbstractWagon::localUpdate(int elapsedMilliseconds)
{
    auto travelledDistance = m_train->travelledDistance() - m_positionOffset;

    QVector3D t = m_train->track()->tangentAt(travelledDistance);
    float angleY = 90.f + atan2(t.x(), t.z()) * 180.f / MathUtil::PI;
    setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.f, 1.f, 0.f), angleY));

    QVector3D trackOffset(0.f, 1.2f, 0.f);
    setPosition(m_train->track()->positionAt(travelledDistance) + trackOffset);
    KinematicPhysicsObject::localUpdate(elapsedMilliseconds);
}

void AbstractWagon::adjustCamera()
{
    if(!m_camera)
    {
        return;
    }

    Camera & camera = *m_camera.get();

    auto center = m_position + m_rotation.rotatedVector(m_lockedCenterOffset);
    camera.setCenter(setCenter(center));

    QQuaternion objectAngle = m_rotation * QQuaternion::fromAxisAndAngle(worldFront(), -20.f);
    auto vA = m_lockedEyeAngle.rotatedVector(QVector3D(1.f, 1.f, 1.f)).normalized(),
          vB = objectAngle.rotatedVector(QVector3D(1.f, 1.f, 1.f)).normalized();
    float angle = acos(QVector3D::dotProduct(vA, vB));
    float f = MathUtil::linstep(MathUtil::PI / 4, MathUtil::PI / 3, angle);
    m_lockedEyeAngle = QQuaternion::slerp(m_lockedEyeAngle, objectAngle, f);

    camera.setEye(center + m_lockedEyeAngle.rotatedVector(QVector3D(0.f, 0.f, -5.f)));
}

void AbstractWagon::moveEvent(QVector3D /*movement*/)
{

}

void AbstractWagon::rotateEvent(QVector2D rotation)
{
    auto viewDirection = (center() - eye()).normalized();
    auto viewNormal = QVector3D::normal(viewDirection, up());
    // "x rotation" -> rotate around up vector
    auto rotation_x = QQuaternion::fromAxisAndAngle(up(), -rotation.x());
    // "y rotation" -> rotation around "the vector pointing to the right"
    auto rotation_y = QQuaternion::fromAxisAndAngle(viewNormal, -rotation.y());
    auto rotation_total = rotation_x * rotation_y;

    m_lockedEyeAngle *= rotation;
}

float AbstractWagon::maxHealth() const
{
    return 100.f;
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

float AbstractWagon::weight() const
{
    // weight in metric tons
    return 1.f;
}

bool AbstractWagon::isDisabled() const
{
    return m_disabled;
}

void AbstractWagon::setPositionOffset(float accumulatedOffset)
{
    m_positionOffset = accumulatedOffset;
}

}//namespace terminus
