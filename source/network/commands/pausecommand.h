#pragma once

#include "abstractcommand.h"

namespace terminus
{

/*!
 * \brief The PauseCommand pauses or unpauses the game.
 */
class PauseCommand : public AbstractCommand
{
public:
    PauseCommand(Timer::TimerMSec timeStamp, bool pause);
    PauseCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject);

    virtual QJsonObject toJson() const override;
    virtual void doWork() override;

    virtual Commands commandType() const override { return Commands::Command_Pause; }

    bool pause() const { return m_pause; }
protected:

    bool m_pause;
};

} // namespace terminus
