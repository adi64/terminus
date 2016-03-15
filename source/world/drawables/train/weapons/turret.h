#pragma once

#include <world/physics/kinematicphysicsobject.h>

namespace terminus
{
class Weapon;

class Turret : public KinematicPhysicsObject
{

public:
    Turret(World & world, Weapon * parent, std::string geometry, std::string material);
    virtual ~Turret();

public:
    virtual void localUpdate() override;

protected:
    virtual short myCollisionType() const override;
    virtual short possibleCollisionTypes() const override;
};

} //terminus
