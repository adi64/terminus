#include "abstractwagon.h"

namespace terminus
{

AbstractWagon::AbstractWagon(Scene *scene)
    : AbstractGraphicsObject(scene)
{
}

float AbstractWagon::length()
{
    return 1.f;
}

void AbstractWagon::setPositionOffset(float accumulatedOffset)
{
    m_positionOffset = accumulatedOffset;
}

}
