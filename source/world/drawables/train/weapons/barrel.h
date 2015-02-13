#pragma once

#include <world/physics/kinematicphysicsobject.h>

namespace terminus
{

class Barrel : public KinematicPhysicsObject
{

public:
    Barrel(std::shared_ptr<Scene> scene, std::string geometry, std::string material);
    ~Barrel();

public:
    virtual void update(int elapsedMilliseconds, QVector3D position, QQuaternion rotation);

};

} //terminus
