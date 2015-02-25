#include "weaponwagon.h"

#include <QDebug>

#include <world/world.h>
#include <resources/resourcemanager.h>
#include <resources/soundmanager.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>
#include <world/drawables/projectile.h>
#include <world/drawables/train/train.h>

//TODO FIXME
#include <network/networkmanager.h>
#include <network/commands/projectilefiredcommand.h>

namespace terminus
{

WeaponWagon::WeaponWagon(World & world, Train * train)
: AbstractWagon(world, train)
, m_elapsedMilliseconds(0)
, m_chargeProjectile(false)
, m_reloadProjectile(false)
, m_force(0.0f)
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
    if(isDisabled() || m_reloadProjectile)
    {
        return;
    }

    QVector3D worldProjectileForce = m_train->playerCamera().normalizedAimVector() * m_force;
    SoundManager::getInstance()->playSound("shot");
    fire(worldProjectileForce);

    m_elapsedMilliseconds = 0;
    m_chargeProjectile = false;
    m_reloadProjectile = true;
}

void WeaponWagon::primaryActionDebug()
{
    QVector3D worldProjectileForce = m_train->playerCamera().normalizedAimVector() * 4000.0;

    fire(worldProjectileForce);
}

void WeaponWagon::setChargeProjectile(bool charge)
{
    m_chargeProjectile = charge;
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
        }
    );

    SoundManager::getInstance()->playSound("shot");

    // woo network!
    auto command = ProjectileFiredCommand(TimeStamp(0), worldProjectilePosition, force);
    m_world.networkManager().sendMessage(&command);
}

bool WeaponWagon::isReloading() const
{
    return m_reloadProjectile;
}

void WeaponWagon::localUpdate(int elapsedMilliseconds)
{
    if(m_chargeProjectile && !m_reloadProjectile)
    {
        if(m_elapsedMilliseconds < 3000)
        {
            m_elapsedMilliseconds += elapsedMilliseconds;
        }

        m_force = m_elapsedMilliseconds * 2.0;
    }
    if(m_reloadProjectile)
    {
        m_elapsedMilliseconds += elapsedMilliseconds;
        if(m_elapsedMilliseconds > 5000)
        {
            m_reloadProjectile = false;
            m_elapsedMilliseconds = 0;
        }
    }

    AbstractWagon::localUpdate(elapsedMilliseconds);
}

void WeaponWagon::localRenderSetup(QOpenGLFunctions& gl, Program & program) const
{
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
}

float WeaponWagon::length() const
{
    return 7.5f;
}

} //namespace terminus
