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
    , m_targetEnemyWagon(nullptr)
{

}

void AIPlayer::update()
{
    AbstractPlayer::update();

    if(selectedWagon()->isDisabled())
    {
        switchWagon();
        return;
    }

    auto focusedWeaponWagon = dynamic_cast<WeaponWagon*>(selectedWagon());
    if(focusedWeaponWagon)
    {
        if(!focusedWeaponWagon->isOnCooldown())
        {
            fire();
        }
        else
        {
            switchWagon();
        }
        return;
    }

    auto focusedRepairWagon = dynamic_cast<RepairWagon*>(selectedWagon());
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

void AIPlayer::fire()
{
    // find target
    if(!m_targetEnemyWagon)
    {
        m_targetEnemyWagon = m_enemyTrain->wagonAt(rand() % m_enemyTrain->size());
    }

    auto aimDirection = (m_targetEnemyWagon->position() - selectedWagon()->position());

    // more up force
    aimDirection += QVector3D(0.0f, 1.0f, 0.0f) * (aimDirection.length() * 0.01f);

    auto normalizedAimDirection = aimDirection.normalized();

    // set camera position accordingly
    m_camera.setEye(m_camera.center() - normalizedAimDirection);

    selectedWagon()->primaryAction();
}


}//namespace terminus
