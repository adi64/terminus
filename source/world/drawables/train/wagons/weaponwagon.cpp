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

namespace terminus
{

WeaponWagon::WeaponWagon(World & world, Train * train)
: AbstractWagon(world, train)
, m_chargeProjectile(false)
, m_chargeTime(0)
, m_reloadProjectile(false)
, m_reloadTime(0)
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

    auto force = 4000.f;
    auto aimDeviation = getAimDeviation();

    QVector3D worldProjectileForce = (m_train->playerCamera().normalizedAimVector() + aimDeviation) * force;
    SoundManager::getInstance()->playSound("shot");
    fire(worldProjectileForce);

    m_chargeTime = 0;
    m_chargeProjectile = false;
    m_reloadProjectile = true;
}

void WeaponWagon::primaryActionDebug()
{
    auto force = 4000.0;
    auto aimDeviation = getAimDeviation();

    QVector3D worldProjectileForce = (m_train->playerCamera().normalizedAimVector() + aimDeviation) * force;

    fire(worldProjectileForce);
}

void WeaponWagon::setChargeProjectile(bool charge)
{
    m_chargeProjectile = charge;
}

void WeaponWagon::fire(QVector3D force)
{
    auto relativeProjectilePosition = QVector3D(0.0f, 4.0f, 0.0f);
    QVector3D worldProjectilePosition = KinematicPhysicsObject::position() + KinematicPhysicsObject::rotation().rotatedVector(relativeProjectilePosition);

    m_world.scheduleAction(
        [this, worldProjectilePosition, force]()
        {
            auto projectile = new Projectile(m_world);
            projectile->moveTo(worldProjectilePosition);
            projectile->applyForce(force);
            m_world.addNode(projectile);
        }
    );
}

bool WeaponWagon::isReloading() const
{
    return m_reloadProjectile;
}

void WeaponWagon::localUpdate(int elapsedMilliseconds)
{
    if(m_chargeProjectile && !m_reloadProjectile)
    {
        if(m_chargeTime < maxChargeMilliseconds())
        {
            if(m_chargeTime + elapsedMilliseconds > maxChargeMilliseconds())
            {
                m_chargeTime = maxChargeMilliseconds();
            }
            else
            {
                m_chargeTime += elapsedMilliseconds;
            }
        }
    }

    if(m_reloadProjectile)
    {
        m_reloadTime += elapsedMilliseconds;
        if(m_reloadTime > 5000)
        {
            m_reloadProjectile = false;
            m_reloadTime = 0;
        }
    }

    std::string materialName = "base_Blue";
    if(currentHealth() <= 0.0f)
    {
        materialName = "base_Grey";
    }

    m_material = ResourceManager::getInstance()->getMaterial(materialName);

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

WagonType WeaponWagon::wagonType() const
{
    return WEAPON_WAGON;
}

unsigned int WeaponWagon::chargeTime() const
{
    return m_chargeTime;
}

unsigned int WeaponWagon::reloadTime() const
{
    return m_reloadTime;
}

unsigned int WeaponWagon::maxChargeMilliseconds() const
{
    return 3000;
}

QVector3D WeaponWagon::getAimDeviation() const
{
    auto localX = QVector3D(1.0, 0.0, 0.0);

    auto globalY = QVector3D(0.0, 1.0, 0.0);

    auto globalX = QVector3D::normal(localX, globalY);

    auto aimDeviationFactorX = 0.5 * (0.5 - (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)));
    auto aimDeviationFactorY = 0.25 * (0.5 - (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)));

    auto aimDeviation = globalX * aimDeviationFactorX + globalY * aimDeviationFactorY;

    // but only a bit of that will be applied - depending on charge
    auto appliedDeviationFactor = 1.0 - (static_cast<float>(m_chargeTime) / static_cast<float>(maxChargeMilliseconds()));

    auto appliedDeviation = aimDeviation * appliedDeviationFactor;

    return appliedDeviation;
}

} //namespace terminus
