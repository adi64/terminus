#pragma once

#include "abstractgraphicsobject.h"

namespace terminus
{

class Track : public AbstractGraphicsObject
{
public:
    Track(Scene *scene);

    void render(QOpenGLFunctions &gl, int elapsedMilliseconds);
    QVector3D positionAt(double distance);
    QVector3D tangentAt(double distance);
};

}
