#pragma once

#include "abstractwagon.h"

#include <world/drawables/train/weapons/weapon.h>

namespace terminus
{

class WeaponWagon : public AbstractWagon
{
public:
    WeaponWagon(World & world, Train *train);
    virtual ~WeaponWagon();

    virtual void localUpdate() override;

    virtual void primaryAction() override;
    virtual void primaryActionDebug() override;
    virtual float cooldownTime() const override;
    virtual WagonType wagonType() const override;

public:
    void setAimVector(const QVector3D & aimVector);
    void setWeapon(Weapon * weapon);
    Weapon * weapon();

protected:
    /*!
     * \brief Spawn a projectile and set velocity
     * \param velocity Velocity vector in global coordinates (will not be rotated to match object's rotation)
     *
     * Schedules a projectile spawn (for next frame) with some position offset, copies the current velocity to the projectile and applies the given velocity to it.
     */
    void fire(QVector3D velocity);

    virtual QVector3D aimVector();

    virtual void doForAllChildren(std::function<void(AbstractGraphicsObject &)> callback) override;

protected:
    std::unique_ptr<Weapon> m_weapon;
};

}
