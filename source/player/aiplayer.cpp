#include "aiplayer.h"

#include <QDebug>

#include <world/drawables/train/wagons/weaponwagon.h>
#include <world/drawables/train/wagons/repairwagon.h>

namespace terminus
{

AIPlayer::AIPlayer(std::shared_ptr<Train> myTrain, std::shared_ptr<Train> enemyTrain)
    : m_myTrain(myTrain)
    , m_enemyTrain(enemyTrain)
    , m_focusedWagonIndex(0)
    , m_chargingMilliseconds(0)
{

}

void AIPlayer::update(int elapsedMilliseconds)
{

    if(m_myTrain->wagonAt(m_focusedWagonIndex)->isDisabled())
    {
        switchWagon();
        return;
    }

    auto focusedWeaponWagon = dynamic_cast<WeaponWagon*>(m_myTrain->wagonAt(m_focusedWagonIndex));
    if(focusedWeaponWagon)
    {
        if(!focusedWeaponWagon->isReloading())
        {
            chargeAndFire(focusedWeaponWagon, elapsedMilliseconds);
        }
        else
        {
            switchWagon();
        }
        return;
    }

    auto focusedRepairWagon = dynamic_cast<RepairWagon*>(m_myTrain->wagonAt(m_focusedWagonIndex));
    if(focusedRepairWagon)
    {
        focusedRepairWagon->primaryAction();

        // repair wagon has no cooldown yet
        switchWagon();
        return;
    }

    // nothing was done at this point -- switch wagons
    switchWagon();

}

void AIPlayer::switchWagon()
{
    if(rand() % 2 == 0)
    {
        // go left
        if(m_focusedWagonIndex + 1 < m_myTrain->size())
        {
            m_focusedWagonIndex++;
        }
    }
    else
    {
        // go right
        if(m_focusedWagonIndex - 1 >= 0)
        {
            m_focusedWagonIndex--;
        }
    }
}

void AIPlayer::chargeAndFire(WeaponWagon *focusedWagon, int elapsedMilliseconds)
{
    // charge for a random amount of time in range 1000-2000ms
    auto chargingThreshold = (rand() % 1000) + 1000; // this will be overwritten on each frame
    if(m_chargingMilliseconds < chargingThreshold)
    {
        focusedWagon->setChargeProjectile(true);
        m_chargingMilliseconds += elapsedMilliseconds;
    }
    else
    {
        // fire!
        auto targetWagon = m_enemyTrain->wagonAt(rand() % m_enemyTrain->size());
        auto aimDirection = (targetWagon->position() - focusedWagon->position());

        // more up force
        aimDirection += QVector3D(0.0f, 1.0f, 0.0f) * (aimDirection.length() * 0.1f);

        focusedWagon->setAimVector(aimDirection);

        focusedWagon->primaryAction();
        m_chargingMilliseconds = 0;
    }
}


}//namespace terminus
