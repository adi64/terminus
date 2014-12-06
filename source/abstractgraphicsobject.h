#pragma once

#include <QVector3D>

namespace terminus
{

class AbstractGraphicsObject
{
public:
    AbstractGraphicsObject();
    virtual void render() = 0;

    QVector3D getWorldPosition();
    QVector3D getRotation();

    void setWorldPosition(QVector3D newPosition);
    void setRotation(QVector3D newRotation);

protected:
    QVector3D m_worldPosition;
    QVector3D m_rotation;
};

}
