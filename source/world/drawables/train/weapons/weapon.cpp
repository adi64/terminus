#include "Weapon.h"

#include <cmath>

#include <resources/soundmanager.h>
#include <util/mathutil.h>

#include <world/world.h>
#include <world/physics/kinematicphysicsobject.h>
#include <world/drawables/train/wagons/weaponwagon.h>
#include <world/drawables/train/weapons/turret.h>
#include <world/drawables/train/weapons/barrel.h>
#include <world/drawables/projectile.h>

#include <network/networkmanager.h>

#include <qDebug>

namespace terminus
{

Weapon::Weapon(World & world, const WeaponWagon * parent)
 : AbstractGraphicsObject(world, parent)
 , m_damage(0.0)
 , m_reloadTime(0.0)
 , m_scattering(0.0)
 , m_thrust(0.0)
 , m_magazineSize(0)
 , m_turret(std::unique_ptr<Turret>(new Turret(world, "engine_right", "base_Red")))
 , m_barrel(std::unique_ptr<Barrel>(new Barrel(world, "weapon_right", "base_Blue")))
{
}

Weapon::~Weapon()
{
}

void Weapon::fire(QVector3D velocity, QVector3D position)
{
    m_world.scheduleAction(
        [this, position, velocity]()
        {
            auto projectile = new Projectile(m_world);
            projectile->moveTo(modelToWorld(position));
            projectile->setLinearVelocity(velocity);
            m_world.addObject(projectile);
            return false;
        }
    );

    m_world.networkManager().sendProjectileFiredCommand(position, velocity);

    SoundManager::getInstance()->playSound("shot");
}

void Weapon::localUpdate()
{
      AbstractGraphicsObject::localUpdate();

      modelMatrix(); //muss das hier sein?

//    QVector3D lookAt = (m_cameraEye - m_cameraCenter).normalized();

//    float angleY = atan2(lookAt.z(), -lookAt.x()) * 180 / MathUtil::PI;
//    float angleX = atan2(-lookAt.y(), lookAt.z()) * 180 / MathUtil::PI;
//    float angleZ = atan2(lookAt.y(), lookAt.x()) * 180 / MathUtil::PI;

//    QQuaternion xz_rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), angleY);
//    QQuaternion y_rotationX = QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), angleX);
//    QQuaternion y_rotationZ = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0), angleZ);

//    m_turret->setRotation(xz_rotation + y_rotationX);
//    m_turret->localUpdate();

//    m_barrel->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), angleY));
//    m_barrel->localUpdate();
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
    callback(*m_turret);
    callback(*m_barrel);
}

bool terminus::Weapon::localRenderEnabled() const
{
    return false;
}

} //terminus
