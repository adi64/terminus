#pragma once

#include <QVector3D>
#include <QOpenGLFunctions>

#include "../abstractgraphicsobject.h"

namespace terminus
{

class Scene;

class Shot : public AbstractGraphicsObject
{
public:
    Shot(Scene * scene, QVector3D position);

    void render(QOpenGLFunctions& gl, int elapsedMilliseconds);

};

}
