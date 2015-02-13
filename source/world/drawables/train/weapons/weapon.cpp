#include "Weapon.h"

#include <world/physics/kinematicphysicsobject.h>
#include <world/drawables/train/weapons/turret.h>
#include <world/drawables/train/weapons/barrel.h>

#include <qDebug>

namespace terminus
{

Weapon::Weapon(std::shared_ptr<Scene> scene)
 : KinematicPhysicsObject(scene)
 , m_damage(0.0)
 , m_reloadTime(0.0)
 , m_scattering(0.0)
 , m_thrust(0.0)
 , m_magazineSize(0)
 , m_turret(std::unique_ptr<Turret>(new Turret(m_scene, "base_Icosahedron", "base_Red")))
 , m_barrel(std::unique_ptr<Barrel>(new Barrel(m_scene, "base_Icosahedron", "base_Red")))
{

}

Weapon::~Weapon()
{
}

void Weapon::fire()
{
    qDebug() << "Aye aye Sir!";
}

void Weapon::update(int elapsedMilliseconds, QVector3D position, QQuaternion rotation)
{
    qDebug() << position;
    m_turret->update(elapsedMilliseconds, position + weaponOffset(), rotation);
    m_barrel->update(elapsedMilliseconds, position + weaponOffset()*2, rotation);
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

} //terminus
