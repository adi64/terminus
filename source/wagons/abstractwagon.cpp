#include "abstractwagon.h"

namespace terminus
{

AbstractWagon::AbstractWagon(Scene *scene)
    : AbstractGraphicsObject(scene)
{
}

float AbstractWagon::length() const
{
    return 1.f;
}

void AbstractWagon::setPositionOffset(float accumulatedOffset)
{
    m_positionOffset = accumulatedOffset;

    // TODO check if still necessary after track is added
    setPosition(QVector3D(m_positionOffset, 0.0, 0.0));
}

}
