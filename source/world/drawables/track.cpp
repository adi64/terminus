#include "track.h"

#include <cassert>

namespace terminus
{

Track::Track(World & world, bool isOtherTrackLeft)
: AbstractGraphicsObject(world)
, m_course(new Polyline)
, m_isOtherTrackLeft(isOtherTrackLeft)
{
}

Polyline & Track::course()
{
    assert(m_course);
    return *m_course;
}

void Track::setCourse(Polyline * course)
{
    m_course.reset(course);
}

bool Track::isRightTrack()
{
    return m_isOtherTrackLeft;
}

bool Track::localRenderEnabled() const
{
    return false;
}

} //namespace terminus
