#include "abstractphysicsobject.h"

#include <QDebug>

#include <world/world.h>
#include <world/physics/bulletworld.h>

namespace terminus
{

AbstractPhysicsObject::AbstractPhysicsObject(World & world)
: AbstractGraphicsObject(world)
, m_bulletWorld(world.bulletWorld())
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

    m_bulletWorld->addRigidBody(m_btRigidBody.get(), myCollisionType(), possibleCollisionTypes());
    m_bulletWorld->addCollisionMapping(m_btRigidBody.get(), this);
}

void AbstractPhysicsObject::deallocatePhysics()
{
    m_bulletWorld->removeRigidBody(m_btRigidBody.get());
    m_bulletWorld->removeCollisionMapping(m_btRigidBody.get());
}

void AbstractPhysicsObject::onCollisionWith(AbstractPhysicsObject *other)
{

}

void AbstractPhysicsObject::setSpawnedLocally(bool spawnedLocally)
{
    m_spawnedLocally = spawnedLocally;
}

}
