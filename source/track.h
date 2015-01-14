#pragma once

#include "abstractgraphicsobject.h"

namespace terminus
{

class Track : public AbstractGraphicsObject
{
public:
    Track(std::shared_ptr<Scene> scene, QVector3D startPosition, QVector3D endPosition);

    void render(QOpenGLFunctions &gl) const override;
    QVector3D positionAt(double distance);
    QVector3D tangentAt(double distance);

protected:
    QVector3D m_startPosition;
    QVector3D m_endPosition;
};

}
