#pragma once

#include <world/physics/kinematicphysicsobject.h>

namespace terminus
{

class WeaponWagon;
class AbstractTurret;
class AbstractBarrel;

class Abstractweapon : public KinematicPhysicsObject
{

public:
    Abstractweapon(std::shared_ptr<Scene> scene);
    virtual ~Abstractweapon();

public:
    float damage();
    float reloadTime();
    float scattering();
    float thrust();
    int magazineSize();

    void setDamage(float amount);
    void setReloadTime(float amount);
    void setScattering(float amount);
    void setThrust(float amount);
    void setMagazineSize(int amount);

protected:
    float m_damage;
    float m_reloadTime;
    float m_scattering;
    float m_thrust;
    int m_magazineSize;

protected:
    std::unique_ptr<AbstractTurret> m_turret;
    std::unique_ptr<AbstractBarrel> m_barrel;

};

} //namespace
