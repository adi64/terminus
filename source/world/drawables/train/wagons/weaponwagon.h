#pragma once

#include "abstractwagon.h"

namespace terminus
{

/*!
 * \brief The WeaponWagon class provides a wagon type, which primary action shoots projectiles in the faced direction.
 */
class WeaponWagon : public AbstractWagon
{
public:
    WeaponWagon(Game & world, Train *train);
    virtual ~WeaponWagon();

    virtual void localUpdate() override;

    virtual void primaryActionInternal() override;

    virtual float cooldownTime() const override;

    virtual WagonType wagonType() const override;

protected:
    /*!
     * \brief Spawn a projectile and set velocity
     * \param velocity Velocity vector in global coordinates (will not be
     * rotated to match object's rotation)
     *
     * Schedules a projectile spawn (for next frame) with some position offset,
     * copies the current velocity to the projectile and applies the given velocity to it.
     */
    void fire(QVector3D velocity);

    /*!
     * \brief Returns the normalized aim vector
     *
     * The aim vector is computed by subtracting the controlling player's
     * camera eye (in world coordinates) from the camera center (in world
     * coordinates) and normalizing the result.
     */
    virtual QVector3D aimVector();
};

}
