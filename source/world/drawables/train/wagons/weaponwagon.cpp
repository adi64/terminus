#include "weaponwagon.h"

#include <QDebug>

#include <resources/resourcemanager.h>
#include <resources/soundmanager.h>
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
: AbstractWagon(world, train)
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    if(m_train->track()->isRightTrack())
    {
        m_geometry = ResourceManager::getInstance()->getGeometry("weapon_right");
    }
    else
    {
        m_geometry = ResourceManager::getInstance()->getGeometry("weapon_left");
    }
    m_material = ResourceManager::getInstance()->getMaterial("base_weaponMat");

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
}

void WeaponWagon::fire(QVector3D velocity)
{
    auto projectilePosition = modelToWorld(localCameraCenter());
    auto projectileVelocity = velocity + (worldFront() * m_train->velocity() * 1000.0f);

    m_world.scheduleAction(
        [this, projectilePosition, projectileVelocity]()
        {
            auto projectile = new Projectile(m_world);
            projectile->moveTo(projectilePosition);
            projectile->setLinearVelocity(projectileVelocity);
            m_world.addObject(projectile);
            return false;
        }
    );

    m_world.networkManager().sendProjectileFiredCommand(projectilePosition, projectileVelocity);

    SoundManager::getInstance()->playSound("shot");
}

QVector3D WeaponWagon::aimVector()
{
    return (modelToWorld(localCameraCenter()) - modelToWorld(localCameraEye())).normalized();
}

float WeaponWagon::cooldownTime() const
{
    return 3000.f;
}

WagonType WeaponWagon::wagonType() const
{
    return WEAPON_WAGON;
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
}

} //namespace terminus
