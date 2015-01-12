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

void DynamicPhysicsObject::applyForce(const QVector3D &force)
{
    m_bullet_rigidBody->applyCentralForce(btVector3(force.x(), force.y(), force.z()));
}

}
