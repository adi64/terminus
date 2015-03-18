#pragma once

#include <network/commands/abstractcommand.h>

namespace terminus
{

/*!
 * \brief The GameEndedCommand signals that the remote game detected a
 * winning or losing condition.
 * It will display a victory or defeat screen accordingly.
 */
class GameEndedCommand : public AbstractCommand
{
public:
    GameEndedCommand(Timer::TimerMSec timeStamp, bool youWin);
    GameEndedCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject);

    virtual QJsonObject toJson() const override;
    virtual void doWork() override;

    virtual Commands commandType() const override { return Commands::Command_GameEnded; }

    bool youWin() const { return m_youWin; }

protected:
    bool m_youWin;
};

} // namespace terminus
