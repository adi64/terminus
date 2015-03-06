#pragma once

#include "abstractcommand.h"

namespace terminus
{

class ProjectileHitCommand : public AbstractCommand
{
public:
    ProjectileHitCommand(bool rightTrain, int wagonIndex, float damage);
    ProjectileHitCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject);

    virtual QJsonObject toJson() const override;
    virtual void doWork() override;

    virtual Commands commandType() const override { return Commands::Command_ProjectileHit; }

    bool rightTrain() const { return m_rightTrain; }
    int wagonIndex() const { return m_wagonIndex; }
    float damage() const { return m_damage; }
protected:

    bool m_rightTrain;
    int m_wagonIndex;
    float m_damage;
};

} // namespace terminus
