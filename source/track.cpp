#include "track.h"

#include <QDebug>
#include <QVector3D>

namespace terminus
{

Track::Track(Scene *scene)
    : AbstractGraphicsObject(scene)
{

}

void Track::render(QOpenGLFunctions &gl, int elapsedMilliseconds)
{
    // TODO
}

QVector3D Track::positionAt(double distance)
{
    // TODO real implementation
    return QVector3D(distance, 1.0, 0.0);
}

QVector3D Track::tangentAt(double distance)
{
    // TODO real implementation
    return positionAt(distance).normalized();
}

}
