#include "abstractwagon.h"

#include <QVector3D>
#include <QMatrix4x4>

#include "../train.h"
#include "../track.h"

namespace terminus
{

AbstractWagon::AbstractWagon(const std::shared_ptr<Scene> &scene, Train *train)
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

}
