#include "dynamicphysicsobject.h"

#include <QDebug>

#include <world/game.h>

namespace terminus
{

DynamicPhysicsObject::DynamicPhysicsObject(Game & world)
    : AbstractPhysicsObject(world)
{
}

void DynamicPhysicsObject::localUpdate()
{
    btTransform transform = m_btRigidBody->getCenterOfMassTransform();
    btVector3 position = transform.getOrigin();
    btQuaternion rotation = transform.getRotation();

    matrix().setPosition(QVector3D(position.x(), position.y(), position.z()));
    matrix().setRotation(QQuaternion(rotation.getW(), QVector3D(rotation.getX(), rotation.getY(), rotation.getZ())));

    AbstractPhysicsObject::localUpdate();
}

void DynamicPhysicsObject::setLinearVelocity(const QVector3D &velocity)
{
    m_btRigidBody->setLinearVelocity(btVector3(velocity.x(), velocity.y(), velocity.z()));
}

void DynamicPhysicsObject::addLinearVelocity(const QVector3D &velocity)
{
    auto newVelocity = m_btRigidBody->getLinearVelocity() + btVector3(velocity.x(), velocity.y(), velocity.z());
    m_btRigidBody->setLinearVelocity(newVelocity);
}

}
