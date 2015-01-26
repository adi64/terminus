#include "weaponwagon.h"

#include <QDebug>

#include "../scene.h"
#include "../resources/soundmanager.h"
#include "../resources/resourcemanager.h"
#include "../resources/geometry.h"
#include "../resources/material.h"
#include "../resources/program.h"
#include "../projectile.h"

namespace terminus
{

WeaponWagon::WeaponWagon(std::shared_ptr<Scene> scene, Train *train)
: AbstractWagon(scene, train)
, m_chargeProjectile(false)
, m_chargeTime(0)
, m_reloadProjectile(false)
, m_reloadTime(0)
{
    auto myShape = new btBoxShape(btVector3(2.5, 1.0, 1.0));
    m_btRigidBody->setCollisionShape(myShape);
    m_btCollisionShape.reset(myShape);

    m_btRigidBody->setMassProps(1000.0f, btVector3(0.0f, 0.0f, 0.0f));

    m_scene->bullet_world()->addRigidBody(m_btRigidBody.get());
}

void WeaponWagon::primaryAction()
{
    if(!m_reloadProjectile)
    {
        QVector3D worldProjectileForce = QVector3D(m_scene->camera().center() - m_scene->camera().eye()) * m_force;

        fire(worldProjectileForce);

        m_chargeTime = 0;
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
    auto scene = m_scene;

    auto relativeProjectilePosition = QVector3D(0.0f, 0.0f, 3.0f);

    QVector3D worldProjectilePosition = position() + rotation().rotatedVector(relativeProjectilePosition);
    QVector3D worldProjectileForce = QVector3D(m_scene->camera().center() - m_scene->camera().eye()) * 1000.0;

    m_scene->scheduleAction(
        [scene, worldProjectilePosition, worldProjectileForce, this]()
        {
            auto projectile = new Projectile(scene);
            projectile->moveTo(worldProjectilePosition);
            projectile->applyForce(worldProjectileForce);
            scene->addNode(projectile);
        }
    );

    SoundManager::getInstance()->playSound("shot");
}

void WeaponWagon::update(int elapsedMilliseconds)
{
    if(m_chargeProjectile && !m_reloadProjectile)
    {
        if(m_chargeTime < 3000)
        {
            m_chargeTime += elapsedMilliseconds;
        }

        m_force = m_chargeTime / 4.0f;
    }
    if(m_reloadProjectile)
    {
        m_reloadTime += elapsedMilliseconds;
        if(m_reloadTime > 5000)
        {
            m_reloadProjectile = false;
            m_reloadTime = 0;
            qDebug() << "Reload complete!";
        }
    }
    AbstractWagon::update(elapsedMilliseconds);
}

void WeaponWagon::render(QOpenGLFunctions& gl) const
{
    Program & program = **(ResourceManager::getInstance()->getProgram("basicShader"));

    std::string materialName = "base_Blue";
    if(currentHealth() <= 0.0f)
    {
        materialName = "base_Orange";
    }
    Material & material = **(ResourceManager::getInstance()->getMaterial(materialName));

    Geometry & geometry = **(ResourceManager::getInstance()->getGeometry("weapon_weapon"));

    program.bind();

    m_scene->camera().setMatrices(program, modelMatrix());
    material.setUniforms(program);
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
    geometry.setAttributes(program);

    geometry.draw(gl);

    program.release();
}

float WeaponWagon::length() const
{
    return 7.5f;
}

unsigned int WeaponWagon::chargeTime() const
{
    return m_chargeTime;
}

unsigned int WeaponWagon::reloadTime() const
{
    return m_reloadTime;
}

} //namespace terminus
