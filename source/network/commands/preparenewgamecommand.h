#pragma once

#include <QDir>
#include <QJsonArray>
#include <QVector3D>

#include "abstractcommand.h"

namespace terminus
{

class PrepareNewGameCommand : public AbstractCommand
{
public:
    PrepareNewGameCommand(TimeStamp timeStamp, bool isPlayerOne, unsigned int terrainSeed);
    PrepareNewGameCommand(TimeStamp timeStamp, QJsonObject jsonObject);

    virtual QJsonObject toJson() const override;
    virtual void doWork() override;

    virtual Commands commandType() const override;

    bool isPlayerOne() const;
    unsigned int terrainSeed() const;
protected:
    bool m_isPlayerOne;
    unsigned int m_terrainSeed;
};
}
