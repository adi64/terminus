#include "abstractwagon.h"

#include <QVector3D>
#include <QMatrix4x4>

#include "../train.h"
#include "../track.h"

namespace terminus
{

AbstractWagon::AbstractWagon(Scene *scene, Train *train)
    : AbstractGraphicsObject(scene)
    , m_train(train)
{
}

void AbstractWagon::primaryAction()
{

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
