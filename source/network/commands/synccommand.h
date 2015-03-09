#pragma once

#include "abstractcommand.h"

#include <vector>

#include <world/drawables/train/train.h>
#include <world/drawables/train/wagons/abstractwagon.h>

namespace terminus
{

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
