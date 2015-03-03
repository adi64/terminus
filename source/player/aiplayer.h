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
    AIPlayer(World & world, Train *train, Train *enemyTrain);

    virtual void update() override;
protected:
    void switchWagon();
    void fire(WeaponWagon * focusedWagon);
protected:
    Train *m_enemyTrain;
    AbstractWagon* m_targetEnemyWagon;
};

}
