#pragma once

#include <world/drawables/train/wagons/abstractwagon.h>
#include <world/drawables/train/weapons/weapon.h>

namespace terminus
{

class WeaponWagon : public AbstractWagon
{
public:
    WeaponWagon(std::shared_ptr<Scene> scene, Train *train);
    virtual ~WeaponWagon();

    void primaryAction() override;
    void primaryActionDebug() override;

    void update(int elapsedMilliseconds) override;
    void preRender(QOpenGLFunctions& gl, Program & program) const override;

public:
    Weapon * weapon();
    float length() const;

    void setChargeProjectile(bool charge);
    void setWeapon(Weapon * weapon);

protected:
    /*!
     * \brief Spawn a projectile and apply force
     * \param force Force vector in global coordinates (will not be rotated to match object's rotation)
     *
     * Schedules a projectile spawn (for next frame) with some position offset, copies the current velocity to the projectile and applies the given force to it.
     */
    void fire(QVector3D force);

protected:
    int m_elapsedMilliseconds;
    bool m_chargeProjectile;
    bool m_reloadProjectile;
    float m_force;

    std::unique_ptr<Weapon> m_weapon;
};

}
