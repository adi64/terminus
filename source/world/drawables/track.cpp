#include "track.h"

#include <QDebug>
#include <QVector3D>

namespace terminus
{

Track::Track(std::shared_ptr<World> scene, std::unique_ptr<Polyline> controlPoints)
: AbstractGraphicsObject(scene)
, m_course(std::move(controlPoints))
{
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
