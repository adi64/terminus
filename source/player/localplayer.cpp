#include "localplayer.h"

#include <world/camera.h>
#include <network/networkmanager.h>

namespace terminus
{

LocalPlayer::LocalPlayer(World & world, Train *train)
: AbstractPlayer(world, train)
{

}

void LocalPlayer::primaryAction()
{
    AbstractPlayer::primaryAction();
}

void LocalPlayer::primaryActionDebug()
{
    AbstractPlayer::primaryActionDebug();
}

}
