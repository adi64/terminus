#pragma once

#include <network/commands/abstractcommand.h>

namespace terminus
{

/*!
 * \brief The ClientReadyCommand signals to the server that the client has
 * successfully created the World and is ready to start the game.
 * It will subsequently unpause the local and remote game.
 */
class ClientReadyCommand : public AbstractCommand
{
public:
    ClientReadyCommand(Timer::TimerMSec timeStamp);
    ClientReadyCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject);

    virtual QJsonObject toJson() const override;
    virtual void doWork() override;

    virtual Commands commandType() const override { return Commands::Command_ClientReady; }
};

} // namespace terminus
