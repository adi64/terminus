#include "gameendedcommand.h"

#include <game.h>
#include <world/world.h>
#include <player/localplayer.h>
#include <world/drawables/train/train.h>
#include <world/drawables/track.h>

namespace terminus
{

GameEndedCommand::GameEndedCommand(Timer::TimerMSec timeStamp, bool firstPlayerWon)
: AbstractCommand(timeStamp)
, m_firstPlayerWon(firstPlayerWon)
{

}

GameEndedCommand::GameEndedCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject)
: AbstractCommand(timeStamp)
{
    m_firstPlayerWon = jsonObject.value("firstPlayerWon").toBool();
}

QJsonObject GameEndedCommand::toJson() const
{
    QJsonObject jsonObject;
    jsonObject.insert("firstPlayerWon", m_firstPlayerWon);
    return jsonObject;}

void GameEndedCommand::doWork()
{
    auto isFirstPlayer = m_game->world().localPlayer().train()->track()->isOtherTrackLeft();

    if((isFirstPlayer && firstPlayerWon()) || (!isFirstPlayer && !firstPlayerWon()))
    {
        QMetaObject::invokeMethod(m_game, "winGame", Qt::AutoConnection);
    }
    else
    {
        QMetaObject::invokeMethod(m_game, "loseGame", Qt::AutoConnection);
    }
}


} // namespace terminus
