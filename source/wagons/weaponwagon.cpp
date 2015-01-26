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
        QVector3D worldProjectileForce = m_normalizedAimVector * m_force;

        fire(worldProjectileForce);

        m_elapsedMilliseconds = 0;
    }

    m_chargeProjectile = false;
    m_reloadProjectile = true;
}

void WeaponWagon::primaryActionDebug()
{
    QVector3D worldProjectileForce = m_normalizedAimVector * 1000.0;

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

} //namespace terminus
