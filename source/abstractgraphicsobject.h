#pragma once

#include <memory>

#include <QOpenGLFunctions>
#include <QVector3D>
#include <QMatrix4x4>

#include "scene.h"

namespace terminus
{

class AbstractGraphicsObject
{
public:
    AbstractGraphicsObject(std::shared_ptr<Scene> scene);
    virtual ~AbstractGraphicsObject();

    virtual void update(int elapsedMilliseconds);
    virtual void render(QOpenGLFunctions & gl) const = 0;

    qreal calculateDistance() const;

    virtual void setPosition(const QVector3D& newPosition);
    virtual QVector3D position() const;
    virtual QVector3D eulerAngles() const;
    virtual QVector3D scaling() const;
    virtual QMatrix4x4 modelMatrix() const;

protected:
    virtual void setEulerAngles(const QVector3D & angles);

protected:
    std::shared_ptr<Scene> m_scene;
    QVector3D m_position;
    QVector3D m_eulerAngles;
    QVector3D m_scaling;
    QMatrix4x4 m_modelMatrix;
};

}
