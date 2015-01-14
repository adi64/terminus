#include "abstractwagon.h"

#include <QVector3D>
#include <QMatrix4x4>

#include "../train.h"
#include "../track.h"

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
    //AbstractPhysicsObject::update(elapsedMilliseconds);

    // compatibility for physics
    setPosition(position());

    auto angles = eulerAngles();

    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(position());
    m_modelMatrix.rotate(angles.x(), angles.y(), angles.z());
    m_modelMatrix.scale(scaling());
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

    // TODO check if still necessary after track is added
    setPosition(QVector3D(m_positionOffset, 0.0, 0.0));
}

QVector3D AbstractWagon::position() const
{
    auto travelledDistance = m_train->travelledDistance() - m_positionOffset;
    return m_train->track()->positionAt(travelledDistance);
}

QVector3D AbstractWagon::tangent() const
{
    auto travelledDistance = m_train->travelledDistance() - m_positionOffset;
    return m_train->track()->tangentAt(travelledDistance);
}

void AbstractWagon::update(int elapsedMilliseconds)
{
    m_position = position();
    QVector3D t = tangent();
    float angleY = 90.f + atan2(t.x(), t.z()) * 180.f / MathUtil::PI;
    m_eulerAngles = QVector3D(0.0f, angleY, 0.0f);
    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(m_position);
    m_modelMatrix.rotate(m_eulerAngles.x(), 1.f, 0.f, 0.f);
    m_modelMatrix.rotate(m_eulerAngles.y(), 0.f, 1.f, 0.f);
    m_modelMatrix.rotate(m_eulerAngles.z(), 0.f, 0.f, 1.f);
}

}
