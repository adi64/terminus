#include "localplayer.h"

#include <player/camera.h>
#include <network/networkmanager.h>

namespace terminus
{

LocalPlayer::LocalPlayer(Game & world, Train *train)
: AbstractPlayer(world, train)
{

}

} // namespace terminus
