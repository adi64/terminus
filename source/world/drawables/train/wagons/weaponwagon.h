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
    virtual float cooldownRate() const override;
    void setAimVector(const QVector3D & aimVector);

    virtual WagonType wagonType() const override;

public:
    virtual float length() const override;

    Weapon * weapon();
    void setWeapon(Weapon * weapon);

protected:
    /*!
     * \brief Spawn a projectile and set velocity
     * \param velocity Velocity vector in global coordinates (will not be rotated to match object's rotation)
     *
     * Schedules a projectile spawn (for next frame) with some position offset, copies the current velocity to the projectile and applies the given velocity to it.
     */
    void fire(QVector3D velocity);

    virtual void doForAllChildren(std::function<void(AbstractGraphicsObject &)> callback) override;

protected:
    QVector3D m_normalizedAimVector;

    std::unique_ptr<Weapon> m_weapon;
};

}
