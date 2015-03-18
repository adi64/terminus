#include "track.h"

#include <QDebug>
#include <QVector3D>

namespace terminus
{

Track::Track(World & world, bool isOtherTrackLeft)
: AbstractGraphicsObject(world)
, m_course(new Polyline)
, m_isOtherTrackLeft(isOtherTrackLeft)
{
}

void Track::setCourse(Polyline * course)
{
    m_course.reset(course);
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

bool Track::isOtherTrackLeft()
{
    return m_isOtherTrackLeft;
}

bool Track::localRenderEnabled() const
{
    return false;
}

}//namespace terminus
