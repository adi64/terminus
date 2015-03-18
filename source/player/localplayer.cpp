#include "localplayer.h"

#include <world/camera.h>
#include <network/networkmanager.h>

namespace terminus
{

LocalPlayer::LocalPlayer(World & world, Train *train)
: AbstractPlayer(world, train)
{

}

} // namespace terminus
