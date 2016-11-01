#include "weaponwagon.h"

#include <QDebug>

#include <resources/resourcemanager.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>

#include <world/drawables/projectile.h>
#include <world/drawables/track.h>
#include <world/drawables/train/train.h>
#include <world/world.h>

#include <player/abstractplayer.h>

#include <network/networkmanager.h>

namespace terminus
{

WeaponWagon::WeaponWagon(World & world, Train * train)
: AbstractWagon(world, nullptr, train)
, m_weapon(std::unique_ptr<Weapon>(nullptr))
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    //TODO: Differenciation is not that important anymore. Maybe still needed for initial weapon focus
    if(m_train->track()->isRightTrack())
    {
        m_geometry = ResourceManager::getInstance()->getGeometry("weaponWagon_weaponWagon");
    }
    else
    {
        m_geometry = ResourceManager::getInstance()->getGeometry("weaponWagon_weaponWagon");
    }
    m_material = ResourceManager::getInstance()->getMaterial("base_weaponMat");

    setWeapon(new Weapon(world, this));

    QVector3D bb = (maxBB() - minBB()) / 2.f;
    initializePhysics(new btBoxShape(btVector3(bb.x(), bb.y(), bb.z())), 1000.f);
}

WeaponWagon::~WeaponWagon()
{
    deallocatePhysics();
}

void WeaponWagon::primaryActionInternal()
{
    auto scalarVelocity = 100.f;
    fire(aimVector() * scalarVelocity);

    resetCooldown();
}

void WeaponWagon::fire(QVector3D velocity)
{
    auto projectilePosition = modelToWorld(localCameraCenter());
    auto projectileVelocity = velocity + (worldFront() * m_train->velocity() * 1000.0f);

    weapon()->fire(projectileVelocity, projectilePosition);
}

float WeaponWagon::cooldownTime() const
{
    return 3000.f;
}

WagonType WeaponWagon::wagonType() const
{
    return WagonType::WEAPON_WAGON;
}

void WeaponWagon::bindCameraToWeapon(Camera & camera)
{
    camera.bindTo(m_weapon.get());
}

void WeaponWagon::setWeapon(Weapon *weapon)
{
    m_weapon.reset(weapon);
}

Weapon * WeaponWagon::weapon()
{
    return m_weapon.get();
}

void WeaponWagon::localUpdate()
{
    std::string materialName = "base_weaponMat";
    if(isDisabled())
    {
        materialName = "base_Grey";
    }

    m_material = ResourceManager::getInstance()->getMaterial(materialName);

    AbstractWagon::localUpdate();

    weapon()->localUpdate();
}

QVector3D WeaponWagon::aimVector()
{
    return (modelToWorld(localCameraCenter()) - modelToWorld(localCameraEye())).normalized();
}

void WeaponWagon::doForAllChildren(std::function<void (AbstractGraphicsObject &)> callback)
{
    callback(*m_weapon);
}

} //namespace terminus
