#pragma once

#include <network/commands/abstractcommand.h>

namespace terminus
{

/*!
 * \brief The GameEndedCommand ends the current game and displays a screen
 * indicating the winner.
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
