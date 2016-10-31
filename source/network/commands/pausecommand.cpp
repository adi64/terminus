#include "pausecommand.h"

#include <application.h>
#include <util/timer.h>

namespace terminus
{

PauseCommand::PauseCommand(Timer::TimerMSec timeStamp, bool pause)
: AbstractCommand(timeStamp)
, m_pause(pause)
{

}

PauseCommand::PauseCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject)
: AbstractCommand(timeStamp)
{
    m_pause = jsonObject.value("pause").toBool();
}

QJsonObject PauseCommand::toJson() const
{
    QJsonObject jsonObject;
    jsonObject.insert("pause", m_pause);
    return jsonObject;
}

void PauseCommand::doWork()
{
    m_game->timer().pause(m_pause);
}

} // namespace terminus
