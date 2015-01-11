#include "abstractphysicsobject.h"

#include <QDebug>

#include <bullet/btBulletDynamicsCommon.h>

#include "scene.h"

namespace terminus
{

AbstractPhysicsObject::AbstractPhysicsObject(Scene *scene)
    : AbstractGraphicsObject(scene)
{
}

AbstractPhysicsObject::~AbstractPhysicsObject()
{
    // TODO FIXME: this breaks on teardown because m_scene pointer is dangling
    /*
    if(m_scene && m_scene->bullet_world() && m_bullet_rigidBody.get())
    {
        m_scene->bullet_world()->removeRigidBody(m_bullet_rigidBody.get());
    }
    */
}

void AbstractPhysicsObject::update(int elapsedMilliseconds)
{
    auto angles = eulerAngles();

    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(position());
    m_modelMatrix.rotate(angles.x(), angles.y(), angles.z());
    m_modelMatrix.scale(scaling());
}

void AbstractPhysicsObject::applyForce(const QVector3D &force)
{
    m_bullet_rigidBody->applyCentralForce(btVector3(force.x(), force.y(), force.z()));
}

void AbstractPhysicsObject::setPosition(const QVector3D &newPosition)
{
    AbstractGraphicsObject::setPosition(newPosition);

    btTransform worldTransform;
    m_bullet_rigidBody->getMotionState()->getWorldTransform(worldTransform);

    worldTransform.setOrigin(btVector3(newPosition.x(), newPosition.y(), newPosition.z()));
    m_bullet_rigidBody->getMotionState()->setWorldTransform(worldTransform);
}

QVector3D AbstractPhysicsObject::position() const
{
    auto pos = m_bullet_rigidBody->getWorldTransform().getOrigin();
    return QVector3D(pos.x(), pos.y(), pos.z());
}

QVector3D AbstractPhysicsObject::eulerAngles() const
{
    auto pos = m_bullet_rigidBody->getWorldTransform().getRotation();
    return QVector3D(pos.getX(), pos.getY(), pos.getZ());
    qDebug() << "are these euler angles really correct?";
}

}
