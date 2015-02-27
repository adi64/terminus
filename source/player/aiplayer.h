#pragma once

#include <memory>

#include <world/drawables/train/train.h>

#include "abstractplayer.h"

namespace terminus
{

class WeaponWagon;

class AIPlayer : public AbstractPlayer
{
public:
    AIPlayer(World & world, std::shared_ptr<Train> train, std::shared_ptr<Train> enemyTrain);

    virtual void update() override;
protected:
    void switchWagon();
    void chargeAndFire(WeaponWagon * focusedWagon);
protected:
    std::shared_ptr<Train> m_enemyTrain;
    int m_chargingMilliseconds;
    AbstractWagon* m_targetEnemyWagon;
};

}
