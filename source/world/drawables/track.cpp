#include "track.h"

#include <QDebug>
#include <QVector3D>

namespace terminus
{

Track::Track(std::shared_ptr<Scene> scene, std::unique_ptr<Polyline> controlPoints)
: AbstractGraphicsObject(scene)
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

}//namespace terminus
