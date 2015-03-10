#include "Weapon.h"

#include <world/physics/kinematicphysicsobject.h>
#include <world/drawables/train/weapons/turret.h>
#include <world/drawables/train/weapons/barrel.h>

#include <qDebug>

namespace terminus
{

Weapon::Weapon(World & world)
 : AbstractGraphicsObject(world)
 , m_damage(0.0)
 , m_reloadTime(0.0)
 , m_scattering(0.0)
 , m_thrust(0.0)
 , m_magazineSize(0)
 , m_turret(std::unique_ptr<Turret>(new Turret(world, "base_ico1", "base_Red")))
 , m_barrel(std::unique_ptr<Barrel>(new Barrel(world, "base_ico1", "base_Blue")))
{

}

Weapon::~Weapon()
{
}

void Weapon::fire()
{
    qDebug() << "Aye Sir!";
}

void Weapon::localUpdate()
{
//    setPosition(position);
//    m_turret->localUpdate(position + weaponOffset(), rotation);
//    m_barrel->localUpdate(position + weaponOffset(), rotation);

    AbstractGraphicsObject::localUpdate();
}

QVector3D Weapon::weaponOffset()
{
    return QVector3D(0.0, 1.0, 2.0);
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
    return m_scattering;
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

void terminus::Weapon::doForAllChildren(std::function<void (AbstractGraphicsObject &)> callback)
{
    qDebug() << __FILE__ << __LINE__;
    callback(*m_turret);
    callback(*m_barrel);
}

bool terminus::Weapon::localRenderEnabled() const
{
    return false;
}

} //terminus
