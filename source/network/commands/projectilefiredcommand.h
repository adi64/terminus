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
    ProjectileFiredCommand(TimeStamp timeStamp, QVector3D startPosition, QVector3D movement);
    ProjectileFiredCommand(TimeStamp timeStamp, QJsonObject jsonObject);

    virtual QJsonObject toJson() const override;
    virtual void doWork() override;

    virtual Commands commandType() const override;

    QVector3D startPosition() const;
    QVector3D movement() const;
protected:

    QVector3D m_startPosition;
    QVector3D m_movement;
};
}
