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
    virtual void update(int elapsedMilliseconds, QVector3D position, QQuaternion rotation);
    void preRender(QOpenGLFunctions& gl, Program & program) const override;

};

} //terminus
