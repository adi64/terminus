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

//TODO FIXME
#include <network/networkmanager.h>
#include <network/commands/projectilefiredcommand.h>

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
    if(isDisabled() || m_onCooldown)
    {
        return;
    }

    auto velocityMultiplicator = 100.f;

    QVector3D worldProjectileVelocity = m_train->player().camera().normalizedAimVector() * velocityMultiplicator;
    SoundManager::getInstance()->playSound("shot");
    fire(worldProjectileVelocity);

    m_onCooldown = true;
    m_cooldown = 0.f;
}

void WeaponWagon::primaryActionDebug()
{
    auto velocityMultiplicator = 100.0;

    QVector3D worldProjectileVelocity = m_train->player().camera().normalizedAimVector() * velocityMultiplicator;
    fire(worldProjectileVelocity);
}

void WeaponWagon::fire(QVector3D velocity)
{
    auto relativeProjectilePosition = QVector3D(0.0f, 4.0f, 0.0f);
    QVector3D worldProjectilePosition = position() + rotation().rotatedVector(relativeProjectilePosition);

    auto totalVelocity = velocity + (worldFront() * m_train->velocity() * -1000.0f);

    m_world.scheduleAction(
        [this, worldProjectilePosition, totalVelocity]()
        {
            auto projectile = new Projectile(m_world);
            projectile->moveTo(worldProjectilePosition);
            projectile->setLinearVelocity(totalVelocity);
            m_world.addNode(projectile);
            return false;
        }
    );

    SoundManager::getInstance()->playSound("shot");

    // woo network!
    auto command = ProjectileFiredCommand(worldProjectilePosition, totalVelocity);
    m_world.networkManager().sendMessage(&command);
}

float WeaponWagon::cooldownRate() const
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
