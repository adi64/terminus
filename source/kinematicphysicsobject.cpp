#include "kinematicphysicsobject.h"

#include <QDebug>

#include <bullet/btBulletDynamicsCommon.h>

#include "scene.h"

namespace terminus
{

KinematicPhysicsObject::KinematicPhysicsObject(std::shared_ptr<Scene> scene)
    : AbstractPhysicsObject(scene)
{
    m_bullet_rigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
}

}
