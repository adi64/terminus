#include "kinematicphysicsobject.h"

#include <QDebug>

#include "scene.h"

namespace terminus
{

KinematicPhysicsObject::KinematicPhysicsObject(std::shared_ptr<Scene> scene)
    : AbstractPhysicsObject(scene)
{
    m_btRigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
}

void KinematicPhysicsObject::update(int elapsedMilliseconds)
{
    QVector3D pos = position();
    QQuaternion rot = rotation();

    btTransform transform = m_btRigidBody->getCenterOfMassTransform();
    transform.setOrigin(btVector3(pos.x(), pos.y(), pos.z()));
    transform.setRotation(btQuaternion(rot.x(), rot.y(), rot.z(), rot.scalar()));

    AbstractPhysicsObject::update(elapsedMilliseconds);
}

}
