#pragma once

#include <world/physics/kinematicphysicsobject.h>

namespace terminus
{

class Turret : public KinematicPhysicsObject
{

public:
    Turret(World & world, std::string geometry, std::string material);
    ~Turret();

public:
    virtual void localUpdate() override;

protected:
    virtual short myCollisionType() const override;
    virtual short possibleCollisionTypes() const override;
};

} //terminus
