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

}
