#include "abstractphysicsobject.h"

#include <QDebug>

#include "scene.h"

namespace terminus
{

AbstractPhysicsObject::AbstractPhysicsObject(std::shared_ptr<Scene> scene)
    : AbstractGraphicsObject(scene)
{
}

void AbstractPhysicsObject::moveTo(const QVector3D & newPosition)
{
    btTransform transform = m_btRigidBody->getCenterOfMassTransform();
    transform.setOrigin(btVector3(newPosition.x(), newPosition.y(), newPosition.z()));
    m_btRigidBody->setCenterOfMassTransform(transform);
}

void AbstractPhysicsObject::initializePhysics(btCollisionShape * collisionShape, btScalar mass)
{
    auto inertia = btVector3(0.0f, 0.0f, 0.0f);
    collisionShape->calculateLocalInertia(mass, inertia);

    auto rotationQuaternion = btQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
    auto positionVector = btVector3(0.0f, 0.0f, 0.0f);
    auto motionState = new btDefaultMotionState(btTransform(rotationQuaternion, positionVector));
    auto rigidBodyConstructionInfo = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, collisionShape, inertia);
    m_btRigidBody = std::unique_ptr<btRigidBody>(new btRigidBody(rigidBodyConstructionInfo));

    m_scene->bullet_world()->addRigidBody(m_btRigidBody.get());
}

void AbstractPhysicsObject::deallocatePhysics()
{
    m_scene->bullet_world()->removeRigidBody(m_btRigidBody.get());
}

}
