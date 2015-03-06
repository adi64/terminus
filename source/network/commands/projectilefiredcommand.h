#pragma once

#include <QDir>
#include <QJsonArray>
#include <QVector3D>

#include "abstractcommand.h"

namespace terminus
{

class ProjectileFiredCommand : public AbstractCommand
{
public:
    ProjectileFiredCommand(QVector3D startPosition, QVector3D velocity);
    ProjectileFiredCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject);

    virtual QJsonObject toJson() const override;
    virtual void doWork() override;

    virtual Commands commandType() const override;

    QVector3D startPosition() const;
    QVector3D velocity() const;
protected:

    QVector3D m_startPosition;
    QVector3D m_velocity;
};
}
