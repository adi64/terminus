#include "dynamicphysicsobject.h"

#include <QDebug>

#include <bullet/btBulletDynamicsCommon.h>

#include "scene.h"

namespace terminus
{

DynamicPhysicsObject::DynamicPhysicsObject(const std::shared_ptr<Scene> &scene)
    : AbstractPhysicsObject(scene)
{
}

void DynamicPhysicsObject::update(int elapsedMilliseconds)
{
    auto angles = eulerAngles();

    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(position());
    m_modelMatrix.rotate(angles.x(), angles.y(), angles.z());
    m_modelMatrix.scale(scaling());
}

void DynamicPhysicsObject::applyForce(const QVector3D &force)
{
    m_bullet_rigidBody->applyCentralForce(btVector3(force.x(), force.y(), force.z()));
}

}
