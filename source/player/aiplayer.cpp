#include "aiplayer.h"

#include <QDebug>

#include <util/timer.h>
#include <world/drawables/train/wagons/weaponwagon.h>
#include <world/drawables/train/wagons/repairwagon.h>

namespace terminus
{

AIPlayer::AIPlayer(World & world, Train *train, Train *enemyTrain)
    : AbstractPlayer(world, train)
    , m_enemyTrain(enemyTrain)
    , m_chargingMilliseconds(0)
    , m_targetEnemyWagon(nullptr)
{

}

void AIPlayer::update()
{
    AbstractPlayer::update();

    if(m_train->wagonAt(m_selectedWagonIndex)->isDisabled())
    {
        switchWagon();
        return;
    }

    auto focusedWeaponWagon = dynamic_cast<WeaponWagon*>(m_train->wagonAt(m_selectedWagonIndex));
    if(focusedWeaponWagon)
    {
        if(!focusedWeaponWagon->isReloading())
        {
            chargeAndFire(focusedWeaponWagon);
        }
        else
        {
            switchWagon();
        }
        return;
    }

    auto focusedRepairWagon = dynamic_cast<RepairWagon*>(m_train->wagonAt(m_selectedWagonIndex));
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
        switchToNextWagon();
    }
    else
    {
        switchToPreviousWagon();
    }
}

void AIPlayer::chargeAndFire(WeaponWagon * focusedWagon)
{
    // find target
    if(!m_targetEnemyWagon)
    {
        m_targetEnemyWagon = m_enemyTrain->wagonAt(rand() % m_enemyTrain->size());
    }

    auto aimDirection = (m_targetEnemyWagon->position() - focusedWagon->position());

    // more up force
    aimDirection += QVector3D(0.0f, 1.0f, 0.0f) * (aimDirection.length() * 0.01f);

    auto normalizedAimDirection = aimDirection.normalized();

    // set camera position accordingly
    m_camera.setEye(m_camera.center() - normalizedAimDirection);

    // charge for a random amount of time in range 1000-2000ms
    auto chargingThreshold = (rand() % 1000) + 1000; // this will be overwritten on each frame
    if(m_chargingMilliseconds < chargingThreshold)
    {
        focusedWagon->setChargeProjectile(true);
        m_chargingMilliseconds += m_world.timer().get("frameTimer");
    }
    else
    {
        // fire!
        focusedWagon->primaryAction();
        m_chargingMilliseconds = 0;

        // next target
        //m_targetEnemyWagon = m_targetEnemyWagon = m_enemyTrain->wagonAt(rand() % m_enemyTrain->size());
    }
}


}//namespace terminus
