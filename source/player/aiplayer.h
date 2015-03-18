#pragma once

#include <memory>

#include <world/drawables/train/train.h>

#include "abstractplayer.h"

namespace terminus
{

class WeaponWagon;

/*!
 * \brief The AIPlayer class implements a simple artificial intelligence enemy
 * player.
 */
class AIPlayer : public AbstractPlayer
{
public:
    AIPlayer(World & world, Train *train, Train *enemyTrain);

    /*!
     * \brief Contains the AI logic and determines what to do in this frame.
     *
     * The AI player switches wagons when they are not usable (disabled or on
     * cooldown), uses RepairWagons and uses WeaponWagons by firing Projectiles
     * at randomly selected wagons of the enemy player.
     */
    virtual void update() override;
protected:
    void switchWagon();
    void fire(WeaponWagon * focusedWagon);
protected:
    Train *m_enemyTrain;
    AbstractWagon* m_targetEnemyWagon;
};

}
