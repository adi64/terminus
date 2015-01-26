#pragma once

#include <memory>

#include "train.h"

namespace terminus
{

class WeaponWagon;

class AIPlayer
{
public:
    AIPlayer(std::shared_ptr<Train> myTrain, std::shared_ptr<Train> enemyTrain);

    void update(int elapsedMilliseconds);
protected:
    void switchWagon();
    void chargeAndFire(WeaponWagon* focusedWagon, int elapsedMilliseconds);
protected:
    std::shared_ptr<Train> m_myTrain;
    std::shared_ptr<Train> m_enemyTrain;
    int m_focusedWagonIndex;
    int m_chargingMilliseconds;
};

}
