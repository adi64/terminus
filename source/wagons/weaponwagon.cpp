#include "weaponwagon.h"

#include <QDebug>

#include "../scene.h"
#include "../resources/soundmanager.h"
#include "../resources/resourcemanager.h"
#include "../resources/geometry.h"
#include "../resources/material.h"
#include "../resources/program.h"
#include "../projectile.h"
#include "../train.h"

namespace terminus
{

WeaponWagon::WeaponWagon(std::shared_ptr<Scene> scene, Train *train)
: AbstractWagon(scene, train)
, m_elapsedMilliseconds(0)
, m_chargeProjectile(false)
, m_reloadProjectile(false)
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
    if(isDisabled())
    {
        return;
    }

    if(!m_reloadProjectile)
    {
        auto force = 2000.0;

        auto aimDeviation = getAimDeviation();

        QVector3D worldProjectileForce = (m_normalizedAimVector + aimDeviation) * force;

        fire(worldProjectileForce);

        m_elapsedMilliseconds = 0;
    }

    m_chargeProjectile = false;
    m_reloadProjectile = true;
}

void WeaponWagon::primaryActionDebug()
{
    auto force = 2000.0;

    auto aimDeviation = getAimDeviation();

    QVector3D worldProjectileForce = (m_normalizedAimVector + aimDeviation) * force;

    fire(worldProjectileForce);
}

void WeaponWagon::setChargeProjectile(bool charge)
{
    m_chargeProjectile = charge;
}

void WeaponWagon::setAimVector(const QVector3D &aimVector)
{
    m_normalizedAimVector = aimVector.normalized();
}

void WeaponWagon::fire(QVector3D force)
{
    auto scene = m_scene;

    auto relativeProjectilePosition = QVector3D(0.0f, 4.0f, 3.0f);

    QVector3D worldProjectilePosition = position() + rotation().rotatedVector(relativeProjectilePosition);

    m_scene->scheduleAction(
        [scene, worldProjectilePosition, force, this]()
        {
            auto projectile = new Projectile(scene);
            projectile->moveTo(worldProjectilePosition);
            projectile->applyForce(force);
            scene->addNode(projectile);
        }
    );

    SoundManager::getInstance()->playSound("shot");
}

bool WeaponWagon::isReloading() const
{
    return m_reloadProjectile;
}

void WeaponWagon::update(int elapsedMilliseconds)
{
    if(m_chargeProjectile && !m_reloadProjectile)
    {
        if(m_elapsedMilliseconds < maxChargeMilliseconds())
        {
            m_elapsedMilliseconds += elapsedMilliseconds;
        }
    }
    if(m_reloadProjectile)
    {
        m_elapsedMilliseconds += elapsedMilliseconds;
        if(m_elapsedMilliseconds > 5000)
        {
            m_reloadProjectile = false;
            m_elapsedMilliseconds = 0;
            qDebug() << "Reload complete!";
        }
    }
    std::string materialName = "base_Blue";
    if(currentHealth() <= 0.0f)
    {
        materialName = "base_Orange";
    }
    m_material = ResourceManager::getInstance()->getMaterial(materialName);
    AbstractWagon::update(elapsedMilliseconds);

    if(m_train->isPlayerControlled())
    {
        m_normalizedAimVector = (m_scene->camera().center() - m_scene->camera().eye()).normalized();
    }

    AbstractWagon::update(elapsedMilliseconds);
}

void WeaponWagon::preRender(QOpenGLFunctions& gl, Program & program) const
{
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
}

float WeaponWagon::length() const
{
    return 7.5f;
}

int WeaponWagon::maxChargeMilliseconds() const
{
    return 3000;
}

QVector3D WeaponWagon::getAimDeviation() const
{
    auto localX = QVector3D(1.0, 0.0, 0.0);

    auto globalY = QVector3D(0.0, 1.0, 0.0);

    auto globalX = QVector3D::normal(localX, globalY);

    auto aimDeviationFactorX = 0.5 * (0.5 - ((float)rand() / (float)RAND_MAX));
    auto aimDeviationFactorY = 0.25 * (0.5 - ((float)rand() / (float)RAND_MAX));

    auto aimDeviation = globalX * aimDeviationFactorX + globalY * aimDeviationFactorY;

    // but only a bit of that will be applied - depending on charge
    auto appliedDeviationFactor = 1.0 - ((float)m_elapsedMilliseconds / (float)maxChargeMilliseconds());

    auto appliedDeviation = aimDeviation * appliedDeviationFactor;

    return appliedDeviation;
}

} //namespace terminus
