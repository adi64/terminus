#include "localplayer.h"

#include <world/camera.h>

namespace terminus
{

LocalPlayer::LocalPlayer(World & world, std::shared_ptr<Train> train)
: AbstractPlayer(world, train)
{

}

}
