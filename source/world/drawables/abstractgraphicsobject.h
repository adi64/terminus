#pragma once

#include <memory>

#include <QOpenGLFunctions>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

#include <world/scene.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>

namespace terminus
{

class AbstractGraphicsObject
{
public:
    AbstractGraphicsObject(std::shared_ptr<Scene> scene);
    virtual ~AbstractGraphicsObject();

    virtual void update(int elapsedMilliseconds);
    virtual void render(QOpenGLFunctions & gl) const;
    virtual void preRender(QOpenGLFunctions & gl, Program & program) const;
    virtual void postRender(QOpenGLFunctions & gl, Program & program) const;

    virtual QVector3D worldUp();
    virtual QVector3D worldFront();
    virtual QVector3D worldRight();

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

    std::shared_ptr<std::unique_ptr<Program>> m_program;
    std::shared_ptr<std::unique_ptr<Geometry>> m_geometry;
    std::shared_ptr<std::unique_ptr<Material>> m_material;

    QVector3D m_position;
    QQuaternion m_rotation;
    QVector3D m_scale;
    mutable bool m_modelMatrixChanged;
    mutable QMatrix4x4 m_modelMatrix;
};

}
