#pragma once

#include "abstractcommand.h"

#include <vector>

namespace terminus
{

class Train;

/*!
 * \brief The SyncCommand is used to continuously synchronize two games.
 *
 * It contains the local player train's travelled distance, velocity and
 * health information of the wagons. It also synchronizes the game timers.
 *
 * \sa AbstractWagon, Train and Timer
 */
class SyncCommand : public AbstractCommand
{
public:
    SyncCommand(Timer::TimerMSec timeStamp, const Train & playerTrain);
    SyncCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject);

    virtual QJsonObject toJson() const override;
    virtual void doWork() override;

    virtual Commands commandType() const override { return Commands::Command_Sync; }

    const std::vector<float> & wagonHealthVector() const { return m_wagonHealthVector; }
protected:
    std::vector<float> m_wagonHealthVector;
    float m_velocity;
    float m_travelledDistance;
};

} // namespace terminus
