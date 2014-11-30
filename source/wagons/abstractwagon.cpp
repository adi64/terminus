#include "abstractwagon.h"

namespace terminus
{

AbstractWagon::AbstractWagon()
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
