#include "track.h"

#include <QDebug>
#include <QVector3D>

namespace terminus
{

Track::Track(World & world, std::unique_ptr<Polyline> controlPoints)
: AbstractGraphicsObject(world)
, m_course(std::move(controlPoints))
{
}

QVector3D Track::positionAt(float distance)
{
    return m_course->getPosition(distance);
}

QVector3D Track::tangentAt(float distance)
{
    return m_course->getTangent(distance);
}

float Track::length()
{
    return m_course->length();
}

bool Track::localRenderEnabled() const
{
    return false;
}

}//namespace terminus
