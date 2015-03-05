#include "kinematicphysicsobject.h"

#include <QDebug>

#include <world/world.h>

namespace terminus
{

KinematicPhysicsObject::KinematicPhysicsObject(World & world)
    : AbstractPhysicsObject(world)
{
}

void KinematicPhysicsObject::localUpdate()
{
    QVector3D pos = position();
    QQuaternion rot = rotation();

    btTransform transform = m_btRigidBody->getCenterOfMassTransform();
    transform.setOrigin(btVector3(pos.x(), pos.y(), pos.z()));
    transform.setRotation(btQuaternion(rot.x(), rot.y(), rot.z(), rot.scalar()));
    m_btRigidBody->setCenterOfMassTransform(transform);

    AbstractPhysicsObject::localUpdate();
}

void KinematicPhysicsObject::initializePhysics(btCollisionShape * collisionShape, btScalar mass)
{
    AbstractPhysicsObject::initializePhysics(collisionShape, mass);
    m_btRigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
}

}
