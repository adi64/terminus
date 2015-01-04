#pragma once

#include <QOpenGLFunctions>
#include <QVector3D>
#include <QMatrix4x4>

namespace terminus
{

class Scene;

class AbstractGraphicsObject
{
public:
    AbstractGraphicsObject(Scene* scene);
    virtual void update(int elapsedMilliseconds) = 0;
    virtual void render(QOpenGLFunctions & gl) const = 0;

    qreal calculateDistance() const;

    void setPosition(const QVector3D& newPosition);
    virtual QVector3D position() const;
    QMatrix4x4 modelMatrix() const;
protected:
    Scene *m_scene;
    QVector3D m_position;

    QMatrix4x4 m_modelMatrix;
};

}
