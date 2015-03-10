#pragma once

#include <network/commands/abstractcommand.h>

namespace terminus
{

class GameEndedCommand : public AbstractCommand
{
public:
    GameEndedCommand(Timer::TimerMSec timeStamp, bool firstPlayerWon);
    GameEndedCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject);

    virtual QJsonObject toJson() const override;
    virtual void doWork() override;

    virtual Commands commandType() const override { return Commands::Command_GameEnded; }

    bool firstPlayerWon() const { return m_firstPlayerWon; }
protected:

    bool m_firstPlayerWon;
};

} // namespace terminus
