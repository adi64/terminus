#pragma once

#include <memory>

#include <QVector3D>

#include "abstractgraphicsobject.h"
#include "../../util/polyline.h"

namespace terminus
{

class Track : public AbstractGraphicsObject
{
public:
    Track(std::shared_ptr<Scene> scene, std::unique_ptr<Polyline> controlPoints);

    void render(QOpenGLFunctions &gl) const override;
    QVector3D positionAt(float distance);
    QVector3D tangentAt(float distance);
    float length();

protected:
    std::unique_ptr<Polyline> m_course;
};

}
