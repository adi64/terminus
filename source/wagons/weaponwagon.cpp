#include "weaponwagon.h"

#include <QDebug>

#include "../scene.h"
#include "../resources/soundmanager.h"
#include "../resources/resourcemanager.h"
#include "../resources/geometry.h"
#include "../resources/material.h"
#include "../resources/program.h"
#include "../projectile.h"
#include "../timer.h"

namespace terminus
{

WeaponWagon::WeaponWagon(std::shared_ptr<Scene> scene, Train *train)
: AbstractWagon(scene, train)
, m_chargeProjectile(false)
, m_reloadProjectile(false)
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry("weapon_weapon");
    m_material = ResourceManager::getInstance()->getMaterial("base_Blue");

    initializePhysics(new btBoxShape(btVector3(2.5, 1.0, 1.0)), 1000.f);

    m_reloadTimer = scene->timer().allocateTimer();
}

WeaponWagon::~WeaponWagon()
{
    deallocatePhysics();
}

void WeaponWagon::primaryAction()
{
    if(!m_reloadProjectile)
    {
        auto scene = m_scene;

        auto relativeProjectilePosition = QVector3D(0.0f, 0.0f, 2.0f);

        QVector3D worldProjectilePosition = position() + rotation().rotatedVector(relativeProjectilePosition);
        QVector3D worldProjectileForce = QVector3D(m_scene->camera().center() - m_scene->camera().eye()) * m_force;

        m_scene->scheduleAction(
            [scene, worldProjectilePosition, worldProjectileForce, this]()
            {
                auto projectile = new Projectile(scene);
                projectile->moveTo(worldProjectilePosition);
                projectile->applyForce(worldProjectileForce);
                scene->addNode(projectile);
                return false;
            }
        );

        m_scene->timer().reset(m_reloadTimer);
        qDebug() << "Projectile fired!";
        SoundManager::getInstance()->playSound("shot");
    }

    m_chargeProjectile = false;
    m_reloadProjectile = true;
}

void WeaponWagon::primaryActionDebug()
{
    QVector3D worldProjectileForce = QVector3D(m_scene->camera().center() - m_scene->camera().eye()) * 1000.0;

    fire(worldProjectileForce);
}

void WeaponWagon::setChargeProjectile(bool charge)
{
    m_chargeProjectile = charge;
    if(m_chargeProjectile)
    {
        m_scene->timer().reset(m_reloadTimer);
    }
}

void WeaponWagon::fire(QVector3D force)
{
    auto scene = m_scene;

    auto relativeProjectilePosition = QVector3D(0.0f, 0.0f, 3.0f);

    QVector3D worldProjectilePosition = position() + rotation().rotatedVector(relativeProjectilePosition);
    m_scene->scheduleAction(
        [scene, worldProjectilePosition, force, this]()
        {
            auto projectile = new Projectile(scene);
            projectile->moveTo(worldProjectilePosition);
            projectile->applyForce(force);
            scene->addNode(projectile);
            return false;
        }
    );

    SoundManager::getInstance()->playSound("shot");
}

void WeaponWagon::update()
{
    float elapsed = Timer::seconds(m_scene->timer().get(m_reloadTimer));

    if(m_chargeProjectile && !m_reloadProjectile)
    {
        m_force = elapsed * 250.f;
    }
    if(m_reloadProjectile)
    {
        if(elapsed > 5.f)
        {
            m_reloadProjectile = false;
            qDebug() << "Reload complete!";
        }
    }
    AbstractWagon::update();
}

void WeaponWagon::preRender(QOpenGLFunctions& gl, Program & program) const
{
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
}

float WeaponWagon::length() const
{
    return 7.5f;
}

} //namespace terminus
