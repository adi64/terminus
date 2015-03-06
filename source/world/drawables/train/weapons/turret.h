#pragma once

#include <world/physics/kinematicphysicsobject.h>

namespace terminus
{

class Turret : public KinematicPhysicsObject
{

public:
    Turret(std::shared_ptr<Scene> scene, std::string geometry, std::string material);
    ~Turret();

public:
    virtual void localUpdate(QVector3D position, QQuaternion rotation) override;
};

} //terminus
