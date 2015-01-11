#include "kinematicphysicsobject.h"

#include <QDebug>

#include <bullet/btBulletDynamicsCommon.h>

#include "scene.h"

namespace terminus
{

KinematicPhysicsObject::KinematicPhysicsObject(const std::shared_ptr<Scene> &scene)
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

}
