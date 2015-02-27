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
    AIPlayer(Train *train, Train *enemyTrain);

    virtual void update(int elapsedMilliseconds) override;
protected:
    void switchWagon();
    void chargeAndFire(WeaponWagon* focusedWagon, int elapsedMilliseconds);
protected:
    Train *m_enemyTrain;
    int m_chargingMilliseconds;
    AbstractWagon* m_targetEnemyWagon;
};

}
