#include "abstractphysicsobject.h"

#include <QDebug>

#include <bullet/btBulletDynamicsCommon.h>

#include "scene.h"

namespace terminus
{

AbstractPhysicsObject::AbstractPhysicsObject(std::shared_ptr<Scene> scene)
    : AbstractGraphicsObject(scene)
{
    m_bullet_collisionShape = std::unique_ptr<btCollisionShape>(new btSphereShape(1.0f));

    auto rotationQuaternion = btQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
    auto positionVector = btVector3(0.0f, 0.0f, 0.0f);

    auto motionState = new btDefaultMotionState(btTransform(rotationQuaternion, positionVector));

    // zero mass --> unlimited mass, does not move
    auto mass = btScalar(0.0f);

    auto inertia = btVector3(0.0f, 0.0f, 0.0f);

    m_bullet_collisionShape->calculateLocalInertia(mass, inertia);

    auto rigidBodyConstructionInfo = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, m_bullet_collisionShape.get(), inertia);

    m_bullet_rigidBody = std::unique_ptr<btRigidBody>(new btRigidBody(rigidBodyConstructionInfo));

    // we should not add the rigid body to the scene right now
    // because fundamental changes to it (collision shape, mass, ...)
    // can not be done while the object is added to the scene
    //
    // therefore every object should do this itself after setting the correct values
}

AbstractPhysicsObject::~AbstractPhysicsObject()
{
    m_scene->bullet_world()->removeRigidBody(m_bullet_rigidBody.get());
}

void AbstractPhysicsObject::applyForce(const QVector3D &force)
{
    m_bullet_rigidBody->applyCentralForce(btVector3(force.x(), force.y(), force.z()));
}

void AbstractPhysicsObject::setPosition(const QVector3D &newPosition)
{
    AbstractGraphicsObject::setPosition(newPosition);

    btTransform transform = m_bullet_rigidBody->getCenterOfMassTransform();
    transform.setOrigin(btVector3(newPosition.x(), newPosition.y(), newPosition.z()));
    m_bullet_rigidBody->setCenterOfMassTransform(transform);
}

QVector3D AbstractPhysicsObject::position() const
{
    auto pos = m_bullet_rigidBody->getWorldTransform().getOrigin();
    return QVector3D(pos.x(), pos.y(), pos.z());
}

QVector3D AbstractPhysicsObject::eulerAngles() const
{
//    auto pos = m_bullet_rigidBody->getWorldTransform().getRotation();
//    return QVector3D(pos.getX(), pos.getY(), pos.getZ());
//    qDebug() << "are these euler angles really correct?"; // No, they are not! (TODO)
    return m_eulerAngles;
}

}
