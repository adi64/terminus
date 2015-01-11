#include "kinematicphysicsobject.h"

#include <QDebug>

#include <bullet/btBulletDynamicsCommon.h>

#include "scene.h"

namespace terminus
{

KinematicPhysicsObject::KinematicPhysicsObject(Scene *scene)
    : AbstractPhysicsObject(scene)
{
}

void KinematicPhysicsObject::update(int elapsedMilliseconds)
{
    auto angles = eulerAngles();

    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(position());
    m_modelMatrix.rotate(angles.x(), angles.y(), angles.z());
    m_modelMatrix.scale(scaling());
}

void KinematicPhysicsObject::setPosition(const QVector3D &newPosition)
{
    AbstractGraphicsObject::setPosition(newPosition);

    btTransform worldTransform;
    m_bullet_rigidBody->getMotionState()->getWorldTransform(worldTransform);

    worldTransform.setOrigin(btVector3(newPosition.x(), newPosition.y(), newPosition.z()));
    m_bullet_rigidBody->getMotionState()->setWorldTransform(worldTransform);
}

}
