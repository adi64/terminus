#include "dynamicphysicsobject.h"

#include <QDebug>

#include <world/world.h>

namespace terminus
{

DynamicPhysicsObject::DynamicPhysicsObject(World & world)
    : AbstractPhysicsObject(world)
{
}

void DynamicPhysicsObject::localUpdate()
{
    btTransform transform = m_btRigidBody->getCenterOfMassTransform();
    btVector3 position = transform.getOrigin();
    btQuaternion rotation = transform.getRotation();

    setPosition(QVector3D(position.x(), position.y(), position.z()));
    setRotation(QQuaternion(rotation.getW(), QVector3D(rotation.getX(), rotation.getY(), rotation.getZ())));

    AbstractPhysicsObject::localUpdate();
}

void DynamicPhysicsObject::applyForce(const QVector3D &force)
{
    m_btRigidBody->applyCentralForce(btVector3(force.x(), force.y(), force.z()));
}

void DynamicPhysicsObject::setLinearVelocity(const QVector3D &velocity)
{
    m_btRigidBody->setLinearVelocity(btVector3(velocity.x(), velocity.y(), velocity.z()));
}

}
