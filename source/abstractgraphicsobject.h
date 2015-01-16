#pragma once

#include <memory>

#include <QOpenGLFunctions>
#include <QVector3D>
#include <QQuaternion>
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

    virtual QVector3D position() const;
    virtual QQuaternion rotation() const;
    virtual QVector3D scale() const;
    virtual QMatrix4x4 modelMatrix() const;

protected:
    virtual void setPosition(const QVector3D & position);
    virtual void setRotation(const QQuaternion & eulerAngles);
    virtual void setScale(const QVector3D & scale);
    virtual void setScale(float scale);

protected:
    std::shared_ptr<Scene> m_scene;

    QVector3D m_position;
    QQuaternion m_rotation;
    QVector3D m_scale;

    mutable bool m_modelMatrixChanged;
    mutable QMatrix4x4 m_modelMatrix;
};

}
