#pragma once

#include <QOpenGLFunctions>
#include <QVector3D>

namespace terminus
{

class Scene;

class AbstractGraphicsObject
{
public:
    AbstractGraphicsObject(Scene* scene);
    virtual void render(QOpenGLFunctions& gl, int elapsedMilliseconds) = 0;
    void setPosition(const QVector3D& newPosition);
    virtual QVector3D position() const;
protected:
    Scene *m_scene;
    QVector3D m_position;
};

}
