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
    m_world.networkManager().sendPrimaryActionCommand(m_selectedWagonIndex, m_camera.normalizedLook());
}

void LocalPlayer::primaryActionDebug()
{
    m_world.networkManager().sendPrimaryActionCommand(m_selectedWagonIndex, m_camera.normalizedLook());
}

}
