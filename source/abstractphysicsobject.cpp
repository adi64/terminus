#include "abstractphysicsobject.h"

#include <QDebug>

#include "scene.h"

namespace terminus
{

AbstractPhysicsObject::AbstractPhysicsObject(std::shared_ptr<Scene> scene)
    : AbstractGraphicsObject(scene)
{
    m_btCollisionShape = std::unique_ptr<btCollisionShape>(new btSphereShape(1.0f));

    auto rotationQuaternion = btQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
    auto positionVector = btVector3(0.0f, 0.0f, 0.0f);

    auto motionState = new btDefaultMotionState(btTransform(rotationQuaternion, positionVector));

    // zero mass --> unlimited mass, does not move
    auto mass = btScalar(0.0f);

    auto inertia = btVector3(0.0f, 0.0f, 0.0f);

    m_btCollisionShape->calculateLocalInertia(mass, inertia);

    auto rigidBodyConstructionInfo = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, m_btCollisionShape.get(), inertia);

    m_btRigidBody = std::unique_ptr<btRigidBody>(new btRigidBody(rigidBodyConstructionInfo));

    // we should not add the rigid body to the scene right now
    // because fundamental changes to it (collision shape, mass, ...)
    // can not be done while the object is added to the scene
    //
    // therefore every object should do this itself after setting the correct values
}

AbstractPhysicsObject::~AbstractPhysicsObject()
{
    m_scene->bullet_world()->removeRigidBody(m_btRigidBody.get());
}

void AbstractPhysicsObject::moveTo(const QVector3D & newPosition)
{
    btTransform transform = m_btRigidBody->getCenterOfMassTransform();
    transform.setOrigin(btVector3(newPosition.x(), newPosition.y(), newPosition.z()));
    m_btRigidBody->setCenterOfMassTransform(transform);
}

}
