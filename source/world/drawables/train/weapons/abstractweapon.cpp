#include "abstractweapon.h"

#include <world/physics/kinematicphysicsobject.h>
#include <world/drawables/train/weapons/turret.h>
#include <world/drawables/train/weapons/barrel.h>

namespace terminus
{

Abstractweapon::Abstractweapon(std::shared_ptr<Scene> scene)
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

Abstractweapon::~Abstractweapon()
{
}

float Abstractweapon::damage()
{
    return m_damage;
}

float Abstractweapon::reloadTime()
{
    return m_reloadTime;
}

float Abstractweapon::scattering()
{
    return m_scattering();
}

float Abstractweapon::thrust()
{
    return m_thrust;
}

int Abstractweapon::magazineSize()
{
    return m_magazineSize;
}

void Abstractweapon::setDamage(float amount)
{
    m_damage = amount;
}

void Abstractweapon::setReloadTime(float amount)
{
    m_reloadTime = amount;
}

void Abstractweapon::setScattering(float amount)
{
    m_scattering = amount;
}

void Abstractweapon::setThrust(float amount)
{
    m_thrust = amount;
}

void Abstractweapon::setMagazineSize(int amount)
{
    m_magazineSize = amount;
}

} //terminus
