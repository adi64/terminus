#include "remoteplayer.h"

#include <world/camera.h>

namespace terminus
{

RemotePlayer::RemotePlayer(World & world, std::shared_ptr<Train> train)
: AbstractPlayer(world, train)
{

}

}
