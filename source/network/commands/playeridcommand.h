#pragma once

#include <QDir>
#include <QJsonArray>
#include <QVector3D>

#include "abstractcommand.h"

namespace terminus
{

class PlayerIdCommand : public AbstractCommand
{
public:
    PlayerIdCommand(TimeStamp timeStamp, int ID);
    PlayerIdCommand(TimeStamp timeStamp, QJsonObject jsonObject);

    virtual QJsonObject toJson() const override;
    virtual void doWork() override;

    virtual Commands commandType() const override;

    int ID() const;
protected:
    int m_ID;
};
}
