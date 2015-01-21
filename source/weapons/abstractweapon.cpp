#include "abstractweapon.h"

namespace terminus
{

abstractweapon::abstractweapon(std::shared_ptr<Scene> scene)
    : KinematicPhysicsObject(scene)
{

}

abstractweapon::~abstractweapon()
{

}

}   //terminus

