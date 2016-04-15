#pragma once

#include <QList>

#include <network/commands/abstractcommand.h>
#include <world/drawables/train/wagons/abstractwagon.h>

namespace terminus
{

class SetTrainCommand : public AbstractCommand
{
public:
    SetTrainCommand(Timer::TimerMSec timeStamp, const Train & train);
    SetTrainCommand(Timer::TimerMSec timeStamp, const std::vector<WagonType> & trainConfig);
    SetTrainCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject);

    virtual QJsonObject toJson() const override;
    virtual void doWork() override;

    virtual Commands commandType() const override;

    const QList<int> &trainConfig() const;

protected:
    QList<int> m_trainConfig;
};

}
