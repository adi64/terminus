#pragma once

#include <memory>

#include <QOpenGLFunctions>
#include <QVector3D>
#include <QMatrix4x4>

#include <bullet/btBulletDynamicsCommon.h>

namespace terminus
{

class Scene;

class AbstractGraphicsObject
{
public:
    AbstractGraphicsObject(Scene* scene);

    virtual void update(int elapsedMilliseconds);
    virtual void render(QOpenGLFunctions & gl) const = 0;

    qreal calculateDistance() const;

    void setPosition(const QVector3D& newPosition);
    virtual QVector3D position() const;
    virtual QVector3D eulerAngles() const;
    virtual QVector3D scaling() const;
    virtual QMatrix4x4 modelMatrix() const;
    virtual bool canCollide() const;
protected:
    Scene *m_scene;
    QVector3D m_position;
    QVector3D m_eulerAngles;
    QVector3D m_scaling;
    QMatrix4x4 m_modelMatrix;
    std::unique_ptr<btRigidBody> m_bullet_rigidBody;
};

}
