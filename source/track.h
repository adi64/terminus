#pragma once

#include <memory>

#include <QVector3D>

#include "abstractgraphicsobject.h"
#include "polyline.h"

namespace terminus
{

class Track : public AbstractGraphicsObject
{
public:
    Track(Scene *scene, std::unique_ptr<Polyline> controlPoints);

    void update(int elapsedMilliseconds);
    void render(QOpenGLFunctions &gl) const override;
    QVector3D positionAt(float distance);
    QVector3D tangentAt(float distance);
    float length();

protected:
    std::unique_ptr<Polyline> m_course;
};

}
