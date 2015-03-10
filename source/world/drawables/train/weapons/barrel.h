#pragma once

#include <world/physics/kinematicphysicsobject.h>

namespace terminus
{

class Barrel : public KinematicPhysicsObject
{

public:
    Barrel(World & world, std::string geometry, std::string material);
    ~Barrel();

public:
    virtual void localUpdate() override;

protected:
    virtual short myCollisionType() const override;
    virtual short possibleCollisionTypes() const override;
};

} //terminus
