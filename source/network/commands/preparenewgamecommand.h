#pragma once

#include <QDir>
#include <QJsonArray>
#include <QVector3D>

#include "abstractcommand.h"

namespace terminus
{
/*!
 *\brief The PrepareNewGameCommand gets sent to the client to synchronize the
 * game timer, terrain seed and client player position (left or right track)
 */
class PrepareNewGameCommand : public AbstractCommand
{
public:
    PrepareNewGameCommand(Timer::TimerMSec timeStamp, bool isPlayerOne, unsigned int terrainSeed);
    PrepareNewGameCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject);

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
