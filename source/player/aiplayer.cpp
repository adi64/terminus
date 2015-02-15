#include "aiplayer.h"

#include <QDebug>

#include <world/drawables/train/wagons/weaponwagon.h>
#include <world/drawables/train/wagons/repairwagon.h>

namespace terminus
{

AIPlayer::AIPlayer(std::shared_ptr<Train> train, std::shared_ptr<Train> enemyTrain)
    : AbstractPlayer(train)
    , m_enemyTrain(enemyTrain)
    , m_chargingMilliseconds(0)
    , m_targetEnemyWagon(nullptr)
{

}

void AIPlayer::update(int elapsedMilliseconds)
{
    AbstractPlayer::update(elapsedMilliseconds);

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
            chargeAndFire(focusedWeaponWagon, elapsedMilliseconds);
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

void AIPlayer::chargeAndFire(WeaponWagon *focusedWagon, int elapsedMilliseconds)
{
    // find target
    if(!m_targetEnemyWagon)
    {
        m_targetEnemyWagon = m_enemyTrain->wagonAt(rand() % m_enemyTrain->size());
    }

    auto aimDirection = (m_targetEnemyWagon->position() - focusedWagon->position());

    // more up force
    aimDirection += QVector3D(0.0f, 1.0f, 0.0f) * (aimDirection.length() * 0.1f);

    auto normalizedAimDirection = aimDirection.normalized();

    // set camera position accordingly
    m_camera->setEye(m_camera->center() - normalizedAimDirection);

    // charge time depends on how near we are to the enemy train (3sec max charge, 200m max train distance)
    auto chargingThreshold = (aimDirection.length() / 200.0f) * 3000.0f;
    if(m_chargingMilliseconds < chargingThreshold)
    {
        focusedWagon->setChargeProjectile(true);
        m_chargingMilliseconds += elapsedMilliseconds;
    }
    else
    {
        // fire
        focusedWagon->primaryAction();
        m_chargingMilliseconds = 0;

        // next target
        //m_targetEnemyWagon = m_targetEnemyWagon = m_enemyTrain->wagonAt(rand() % m_enemyTrain->size());
    }
}


}//namespace terminus
