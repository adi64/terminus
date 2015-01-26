#include "aiplayer.h"

#include <QDebug>

#include "wagons/weaponwagon.h"

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
    auto focusedWeaponWagon = dynamic_cast<WeaponWagon*>(m_myTrain->wagonAt(m_focusedWagonIndex));
    if(focusedWeaponWagon && !focusedWeaponWagon->isReloading() && !focusedWeaponWagon->isDisabled())
    {
        // charge weapon and fire

        // charge for a random amount of time in range 1000-2000ms
        auto chargingThreshold = (rand() % 1000) + 1000; // this will be overwritten on each frame
        if(m_chargingMilliseconds < chargingThreshold)
        {
            qDebug() << "charging weapon... (" << m_chargingMilliseconds << " ms)";
            focusedWeaponWagon->setChargeProjectile(true);
            m_chargingMilliseconds += elapsedMilliseconds;
        }
        else
        {
            // fire!
            qDebug() << "firing weapon after " << m_chargingMilliseconds << " ms charging";

            auto targetWagon = m_enemyTrain->wagonAt(rand() % m_enemyTrain->size());
            auto aimDirection = (targetWagon->position() - focusedWeaponWagon->position());

            // more up force
            aimDirection += QVector3D(0.0f, 1.0f, 0.0f) * (aimDirection.length() * 0.1f);

            qDebug() << "aim direction: " << aimDirection;

            focusedWeaponWagon->setAimVector(aimDirection);

            focusedWeaponWagon->primaryAction();
            m_chargingMilliseconds = 0;
        }
    }
    else
    {
        // switch wagon
        if(rand() % 2 == 0)
        {
            // go left
            if(m_focusedWagonIndex + 1 < m_myTrain->size())
            {
                m_focusedWagonIndex++;
                qDebug() << "switching to wagon " << m_focusedWagonIndex;
            }
        }
        else
        {
            // go right
            if(m_focusedWagonIndex - 1 >= 0)
            {
                m_focusedWagonIndex--;
                qDebug() << "switching to wagon " << m_focusedWagonIndex;
            }
        }

    }
}


}//namespace terminus
