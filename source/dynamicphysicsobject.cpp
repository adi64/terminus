#include "dynamicphysicsobject.h"

#include <QDebug>

#include "scene.h"

namespace terminus
{

DynamicPhysicsObject::DynamicPhysicsObject(std::shared_ptr<Scene> scene)
    : AbstractPhysicsObject(scene)
{
}

void DynamicPhysicsObject::update(int elapsedMilliseconds)
{
    btTransform transform = m_btRigidBody->getCenterOfMassTransform();
    btVector3 position = transform.getOrigin();
    btQuaternion rotation = transform.getRotation();

    setPosition(QVector3D(position.x(), position.y(), position.z()));
    setRotation(QQuaternion(rotation.getW(), QVector3D(rotation.getX(), rotation.getY(), rotation.getZ())));

    AbstractPhysicsObject::update(elapsedMilliseconds);
}

void DynamicPhysicsObject::applyForce(const QVector3D &force)
{
    m_btRigidBody->applyCentralForce(btVector3(force.x(), force.y(), force.z()));
}

}
