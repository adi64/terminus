#include "Weapon.h"

#include <world/physics/kinematicphysicsobject.h>
#include <world/drawables/train/weapons/turret.h>
#include <world/drawables/train/weapons/barrel.h>

namespace terminus
{

Weapon::Weapon(std::shared_ptr<Scene> scene)
 : KinematicPhysicsObject(scene)
 , m_damage(0.0)
 , m_reloadTime(0.0)
 , m_scattering(0.0)
 , m_thrust(0.0)
 , m_magazineSize(0)
 , m_turret(std::unique_ptr(nullptr))
 , m_barrel(std::unique_ptr(nullptr))
{

}

Weapon::~Weapon()
{
}

float Weapon::damage()
{
    return m_damage;
}

float Weapon::reloadTime()
{
    return m_reloadTime;
}

float Weapon::scattering()
{
    return m_scattering();
}

float Weapon::thrust()
{
    return m_thrust;
}

int Weapon::magazineSize()
{
    return m_magazineSize;
}

void Weapon::setDamage(float amount)
{
    m_damage = amount;
}

void Weapon::setReloadTime(float amount)
{
    m_reloadTime = amount;
}

void Weapon::setScattering(float amount)
{
    m_scattering = amount;
}

void Weapon::setThrust(float amount)
{
    m_thrust = amount;
}

void Weapon::setMagazineSize(int amount)
{
    m_magazineSize = amount;
}

} //terminus
