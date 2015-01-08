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
    static_cast<btRigidBody*>(m_bullet_rigidBody.get())->applyCentralForce(btVector3(force.x(), force.y(), force.z()));
}

void AbstractPhysicsObject::setPosition(const QVector3D &newPosition)
{

    qDebug() << "setting new position: " << newPosition;

    AbstractGraphicsObject::setPosition(newPosition);

    btTransform worldTransform;
    static_cast<btRigidBody*>(m_bullet_rigidBody.get())->getMotionState()->getWorldTransform(worldTransform);

    auto translation = btVector3(
                newPosition.x() - worldTransform.getOrigin().x(),
                newPosition.y() - worldTransform.getOrigin().y(),
                newPosition.z() - worldTransform.getOrigin().z());

    static_cast<btRigidBody*>(m_bullet_rigidBody.get())->translate(translation);
}

QVector3D AbstractPhysicsObject::position() const
{
    auto pos = static_cast<btRigidBody*>(m_bullet_rigidBody.get())->getWorldTransform().getOrigin();
    return QVector3D(pos.x(), pos.y(), pos.z());
}

QVector3D AbstractPhysicsObject::eulerAngles() const
{
    auto pos = static_cast<btRigidBody*>(m_bullet_rigidBody.get())->getWorldTransform().getRotation();
    return QVector3D(pos.getX(), pos.getY(), pos.getZ());
    qDebug() << "are these euler angles really correct?";
}

}
