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
    if(isDisabled() || m_onCooldown)
    {
        return;
    }

    auto scalarVelocity = 100.f;
    fire(aimVector() * scalarVelocity);

    SoundManager::getInstance()->playSound("shot");

    m_onCooldown = true;
    m_cooldown = 0.f;
}

void WeaponWagon::primaryActionDebug()
{
    auto scalarVelocity = 100.0;
    fire(aimVector() * scalarVelocity);
}

void WeaponWagon::fire(QVector3D velocity)
{
    m_world.scheduleAction(
        [this, velocity]()
        {
            auto projectile = new Projectile(m_world);
            projectile->moveTo(modelToWorld(localCameraCenter()));
            projectile->setLinearVelocity(velocity + (worldFront() * m_train->velocity() * -1000.0f));
            m_world.addNode(projectile);
            return false;
        }
    );
}

QVector3D WeaponWagon::aimVector()
{
    return (modelToWorld(localCameraCenter()) - modelToWorld(localCameraEye())).normalized();
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
