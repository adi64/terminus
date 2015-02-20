#include "weaponwagon.h"

#include <QDebug>

#include <world/scene.h>
#include <resources/resourcemanager.h>
#include <resources/soundmanager.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>
#include <world/drawables/projectile.h>
#include <world/drawables/train/train.h>

namespace terminus
{

WeaponWagon::WeaponWagon(std::shared_ptr<Scene> scene, Train *train)
: AbstractWagon(scene, train)
, m_elapsedMilliseconds(0)
, m_chargeProjectile(false)
, m_reloadProjectile(false)
, m_weapon(std::unique_ptr<Weapon>(nullptr))
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry("weapon_weapon");
    m_material = ResourceManager::getInstance()->getMaterial("base_Blue");

    initializePhysics(new btBoxShape(btVector3(2.5, 1.0, 1.0)), 1000.f);
    setWeapon(new Weapon(m_scene)); //remove later
}

WeaponWagon::~WeaponWagon()
{
    deallocatePhysics();
}

void WeaponWagon::primaryAction()
{
    if(!m_reloadProjectile)
    {
        QVector3D worldProjectileForce = QVector3D(m_scene->camera().center() - m_scene->camera().eye()) * m_force;

        fire(worldProjectileForce);

        m_elapsedMilliseconds = 0;
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
}

void WeaponWagon::fire(QVector3D force)
{
    /*
    auto scene = m_scene;

    auto relativeProjectilePosition = QVector3D(0.0f, 0.0f, 3.0f);
    QVector3D worldProjectilePosition = position() + rotation().rotatedVector(relativeProjectilePosition);

    auto relativeProjectileStartVelocity = QVector3D(1.0f, 0.0f, 0.0f) * m_train->velocity() * 1000.0f;
    auto worldProjectileStartVelocity = rotation().rotatedVector(relativeProjectileStartVelocity);

    m_scene->scheduleAction(
        [=]()
        {
            auto projectile = new Projectile(scene);
            projectile->moveTo(worldProjectilePosition);
            projectile->setLinearVelocity(worldProjectileStartVelocity);
            projectile->applyForce(force);
            scene->addNode(projectile);
        }
    );*/
    weapon()->fire();

    SoundManager::getInstance()->playSound("shot");
}

void WeaponWagon::update(int elapsedMilliseconds)
{
    if(m_chargeProjectile && !m_reloadProjectile)
    {
        if(m_elapsedMilliseconds < 3000)
        {
            m_elapsedMilliseconds += elapsedMilliseconds;
        }

        m_force = m_elapsedMilliseconds / 4.0f;
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
    if(weapon())
    {
        weapon()->update(elapsedMilliseconds, position(), rotation());
    }

    AbstractWagon::update(elapsedMilliseconds);
}

void WeaponWagon::preRender(QOpenGLFunctions& gl, Program & program) const
{
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
}

void WeaponWagon::postRender(QOpenGLFunctions & gl, Program & program) const
{
    program.release();

    if(m_weapon.get())
    {
        m_weapon->render(gl);
    }
}

void WeaponWagon::setWeapon(Weapon * weapon)
{
    m_weapon.reset(weapon);
}

Weapon * WeaponWagon::weapon()
{
    return m_weapon.get();
}

float WeaponWagon::length() const
{
    return 7.5f;
}

} //namespace terminus
