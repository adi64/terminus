#pragma once

#include "abstractcommand.h"

#include <QVector3D>

namespace terminus
{

class PrimaryActionCommand : public AbstractCommand
{
public:
    PrimaryActionCommand(Timer::TimerMSec timeStamp, unsigned int selectedWagonIndex, QVector3D aimDirection);
    PrimaryActionCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject);

    virtual QJsonObject toJson() const override;
    virtual void doWork() override;

    virtual Commands commandType() const override { return Commands::Command_PrimaryAction; }

    QVector3D aimDirection() const { return m_aimDirection; }
protected:

    unsigned int m_selectedWagonIndex;
    QVector3D m_aimDirection;
};

} // namespace terminus
