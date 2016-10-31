#include "kinematicphysicsobject.h"

#include <QDebug>

#include <world/game.h>

namespace terminus
{

KinematicPhysicsObject::KinematicPhysicsObject(Game & world)
    : AbstractPhysicsObject(world)
{
}

void KinematicPhysicsObject::localUpdate()
{
    QVector3D pos = position();
    QQuaternion rot = rotation();

    btTransform transform = m_btRigidBody->getCenterOfMassTransform();
    auto& offset = (minBB() + maxBB()) / 2.f;
    btVector3 originVec(pos.x() + offset.x(), pos.y() + offset.y(), pos.z() + offset.z());
    transform.setOrigin(originVec);
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
