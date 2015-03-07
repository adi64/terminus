#include "weaponwagon.h"

#include <QDebug>

#include <resources/resourcemanager.h>
#include <resources/soundmanager.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>

#include <world/drawables/projectile.h>
#include <world/drawables/train/train.h>
#include <world/world.h>

#include <player/abstractplayer.h>

namespace terminus
{

WeaponWagon::WeaponWagon(World & world, Train * train)
: AbstractWagon(world, train)
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry("weapon_weapon");
    m_material = ResourceManager::getInstance()->getMaterial("base_Blue");

    initializePhysics(new btBoxShape(btVector3(2.5, 1.0, 1.0)), 1000.f);
}

WeaponWagon::~WeaponWagon()
{
    deallocatePhysics();
}

void WeaponWagon::primaryAction()
{
    if(isDisabled() || isOnCooldown())
    {
        return;
    }

    auto velocityMultiplicator = 100.f;

    QVector3D worldProjectileVelocity = m_train->player().camera().normalizedLook() * velocityMultiplicator;
    SoundManager::getInstance()->playSound("shot");
    fire(worldProjectileVelocity);

    resetCooldown();
}

void WeaponWagon::primaryActionDebug()
{
    auto velocityMultiplicator = 100.0;

    QVector3D worldProjectileVelocity = m_train->player().camera().normalizedLook() * velocityMultiplicator;
    fire(worldProjectileVelocity);
}

void WeaponWagon::fire(QVector3D velocity)
{
    auto relativeProjectilePosition = QVector3D(0.0f, 4.0f, 0.0f);
    QVector3D worldProjectilePosition = position() + rotation().rotatedVector(relativeProjectilePosition);

    m_world.scheduleAction(
        [this, worldProjectilePosition, velocity]()
        {
            auto projectile = new Projectile(m_world);
            projectile->moveTo(worldProjectilePosition);
            projectile->setLinearVelocity(velocity + (worldFront() * m_train->velocity() * -1000.0f));
            m_world.addObject(projectile);
            return false;
        }
    );
}

float WeaponWagon::cooldownTime() const
{
    return 3000.f;
}

float WeaponWagon::length() const
{
    return 7.5f;
}

WagonType WeaponWagon::wagonType() const
{
    return WEAPON_WAGON;
}

void WeaponWagon::localUpdate()
{
    std::string materialName = "base_Blue";
    if(isDisabled())
    {
        materialName = "base_Grey";
    }

    m_material = ResourceManager::getInstance()->getMaterial(materialName);

    AbstractWagon::localUpdate();
}

} //namespace terminus
