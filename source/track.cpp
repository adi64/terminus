#include "track.h"

namespace terminus
{

Track::Track(Scene *scene, std::unique_ptr<Polyline> controlPoints)
: AbstractGraphicsObject(scene)
, m_course(std::move(controlPoints))
{
}

void Track::update(int elapsedMilliseconds)
{
    // TODO
}

void Track::render(QOpenGLFunctions &gl) const
{
    // TODO
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

}//namespace terminus
