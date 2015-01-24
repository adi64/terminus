#include "aiplayer.h"

#include <QDebug>

#include "wagons/weaponwagon.h"

namespace terminus
{

AIPlayer::AIPlayer(std::shared_ptr<Train> myTrain, std::shared_ptr<Train> enemyTrain)
    : m_myTrain(myTrain)
    , m_enemyTrain(enemyTrain)
    , m_focusedWagonIndex(1)
{

}

void AIPlayer::update(int elapsedMilliseconds)
{
    if ((rand() % 5) == 0)
    {
        auto focusedWeaponWagon = dynamic_cast<WeaponWagon*>(m_myTrain->wagonAt(m_focusedWagonIndex));
        if(focusedWeaponWagon)
        {
            auto targetWagon = m_enemyTrain->wagonAt(rand() % m_enemyTrain->size());
            auto aimDirection = targetWagon->position() - focusedWeaponWagon->position();
            auto localForce = aimDirection * 100.0f;
            auto globalForce = focusedWeaponWagon->rotation().rotatedVector(localForce);

            focusedWeaponWagon->fire(globalForce);
        }
    }
}


}//namespace terminus
