#pragma once

#include <world/physics/kinematicphysicsobject.h>

namespace terminus
{
class Weapon;

class Barrel : public KinematicPhysicsObject
{

public:
    Barrel(World & world, Weapon * parent, std::string geometry, std::string material);
    virtual ~Barrel();

public:
    virtual void localUpdate() override;

protected:
    virtual short myCollisionType() const override;
    virtual short possibleCollisionTypes() const override;
};

} //terminus
