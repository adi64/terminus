#include "track.h"

#include <QDebug>
#include <QVector3D>

namespace terminus
{

Track::Track(std::shared_ptr<Scene> scene, QVector3D startPosition, QVector3D endPosition)
    : AbstractGraphicsObject(scene)
    , m_startPosition(startPosition)
    , m_endPosition(endPosition)
{
}

void Track::render(QOpenGLFunctions &gl) const
{
    // TODO
}

QVector3D Track::positionAt(double distance)
{
    auto length = (m_endPosition - m_startPosition).length();
    auto normalizedDistance = distance / length;

    auto interpolatedPosition =
        ((1.0 - normalizedDistance) * m_startPosition) + ((normalizedDistance) * m_endPosition);
    return interpolatedPosition;
}

QVector3D Track::tangentAt(double distance)
{
    return positionAt(distance).normalized();
}

}
