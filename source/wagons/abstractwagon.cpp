#include "abstractwagon.h"

#include <QVector3D>
#include <QMatrix4x4>

#include "../train.h"
#include "../track.h"
#include "../mathutil.h"

namespace terminus
{

AbstractWagon::AbstractWagon(std::shared_ptr<Scene> scene, Train *train)
    : KinematicPhysicsObject(scene)
    , m_train(train)
{
}

void AbstractWagon::primaryAction()
{

}

void AbstractWagon::update(int elapsedMilliseconds)
{
    auto travelledDistance = m_train->travelledDistance() - m_positionOffset;

    QVector3D t = m_train->track()->tangentAt(travelledDistance);
    float angleY = 90.f + atan2(t.x(), t.z()) * 180.f / MathUtil::PI;
    setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0, 0.0f), angleY));

    setPosition(m_train->track()->positionAt(travelledDistance));
    KinematicPhysicsObject::update(elapsedMilliseconds);
}

float AbstractWagon::length() const
{
    return 1.f;
}

float AbstractWagon::weight() const
{
    // weight in metric tons
    return 1.0f;
}

void AbstractWagon::setPositionOffset(float accumulatedOffset)
{
    m_positionOffset = accumulatedOffset;
}

}//namespace terminus
