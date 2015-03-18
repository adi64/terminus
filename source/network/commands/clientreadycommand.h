#pragma once

#include "abstractcommand.h"

namespace terminus
{

/*!
 * \brief The ClientReadyCommand signals to the server that the client has
 * successfully created the World and is ready to start the game.
 *
 * \sa NetworkManager::clientReady()
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
