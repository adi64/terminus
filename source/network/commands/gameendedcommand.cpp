#include "gameendedcommand.h"

#include <game.h>
#include <world/world.h>
#include <player/localplayer.h>
#include <world/drawables/train/train.h>
#include <world/drawables/track.h>

namespace terminus
{

GameEndedCommand::GameEndedCommand(Timer::TimerMSec timeStamp, bool youWin)
: AbstractCommand(timeStamp)
, m_youWin(youWin)
{

}

GameEndedCommand::GameEndedCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject)
: AbstractCommand(timeStamp)
{
    m_youWin = jsonObject.value("youWin").toBool();
}

QJsonObject GameEndedCommand::toJson() const
{
    QJsonObject jsonObject;
    jsonObject.insert("youWin", m_youWin);
    return jsonObject;}

void GameEndedCommand::doWork()
{
    if(m_youWin)
    {
        QMetaObject::invokeMethod(m_game, "winGame", Qt::AutoConnection);
    }
    else
    {
        QMetaObject::invokeMethod(m_game, "loseGame", Qt::AutoConnection);
    }
}


} // namespace terminus
