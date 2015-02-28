#include "weaponwagon.h"

#include <QDebug>


#include <resources/resourcemanager.h>
#include <resources/soundmanager.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>
#include <util/timer.h>
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

    auto force = 4000.f;

    QVector3D worldProjectileForce = m_train->player().camera().normalizedAimVector() * force;
    SoundManager::getInstance()->playSound("shot");
    fire(worldProjectileForce);

    m_onCooldown = true;
}

void WeaponWagon::primaryActionDebug()
{
    auto force = 4000.0;

    QVector3D worldProjectileForce = m_train->player().camera().normalizedAimVector() * force;
    fire(worldProjectileForce);
}

void WeaponWagon::fire(QVector3D force)
{
    auto relativeProjectilePosition = QVector3D(0.0f, 4.0f, 0.0f);
    QVector3D worldProjectilePosition = position() + rotation().rotatedVector(relativeProjectilePosition);

    m_world.scheduleAction(
        [this, worldProjectilePosition, force]()
        {
            auto projectile = new Projectile(m_world);
            projectile->moveTo(worldProjectilePosition);
            projectile->applyForce(force);
            m_world.addNode(projectile);
            return false;
        }
    );
}

unsigned int WeaponWagon::maxCooldown() const
{
    return 3000;
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
    Timer::TimerMSec frameDuration = m_world.timer().get("frameTimer");

    if(m_onCooldown)
    {
        m_cooldown += frameDuration;
        if(m_cooldown > maxCooldown())
        {
            m_onCooldown = false;
            m_cooldown = 0;
        }
    }

    std::string materialName = "base_Blue";
    if(isDisabled())
    {
        materialName = "base_Grey";
    }

    m_material = ResourceManager::getInstance()->getMaterial(materialName);

    AbstractWagon::localUpdate();
}

void WeaponWagon::localRenderSetup(QOpenGLFunctions& gl, Program & program) const
{
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
}

} //namespace terminus
