#pragma once

#include <memory>

#include <world/drawables/train/train.h>

#include "abstractplayer.h"

namespace terminus
{

class WeaponWagon;

class AIPlayer : AbstractPlayer
{
public:
    AIPlayer(std::shared_ptr<Train> train, std::shared_ptr<Train> enemyTrain);

    virtual void update(int elapsedMilliseconds) override;
protected:
    void switchWagon();
    void chargeAndFire(WeaponWagon* focusedWagon, int elapsedMilliseconds);
protected:
    std::shared_ptr<Train> m_enemyTrain;
    int m_chargingMilliseconds;
};

}
