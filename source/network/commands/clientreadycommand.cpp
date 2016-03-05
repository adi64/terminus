#include "clientreadycommand.h"

#include <game.h>
#include <network/networkmanager.h>

namespace terminus
{

ClientReadyCommand::ClientReadyCommand(Timer::TimerMSec timeStamp)
: AbstractCommand(timeStamp)
{

}

ClientReadyCommand::ClientReadyCommand(Timer::TimerMSec timeStamp, QJsonObject)
: AbstractCommand(timeStamp)
{

}

QJsonObject ClientReadyCommand::toJson() const
{
    return {};
}

void ClientReadyCommand::doWork()
{
    m_game->networkManager().sendPauseCommand(false);
    m_game->setPaused(false);
}

} // namespace terminus
