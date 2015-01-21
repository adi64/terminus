#pragma once

#include "../kinematicphysicsobject.h"

namespace terminus
{

class abstractweapon : public KinematicPhysicsObject
{

public:
    Abstractweapon(std::shared_ptr<Scene> scene);
    virtual ~Abstractweapon();


};

}   //namespace
