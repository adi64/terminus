#include "weaponwagon.h"

#include <QDebug>

#include "../scene.h"
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
, m_accumulate(false)
{
    auto myShape = new btBoxShape(btVector3(2.5, 1.0, 1.0));
    m_btRigidBody->setCollisionShape(myShape);
    m_btCollisionShape.reset(myShape);

    m_btRigidBody->setMassProps(1000.0f, btVector3(0.0f, 0.0f, 0.0f));

    m_scene->bullet_world()->addRigidBody(m_btRigidBody.get());
}

void WeaponWagon::primaryAction()
{
    auto scene = m_scene;

    auto relativeProjectilePosition = QVector3D(0.0f, 0.0f, 2.0f);
    //auto relativeProjectileForce = QVector3D(0.0f, 200.0f, 300.0f);
    auto relativeProjectileForce = m_force;

    QVector3D worldProjectilePosition = position() + rotation().rotatedVector(relativeProjectilePosition);
    QVector3D worldProjectileForce = rotation().rotatedVector(relativeProjectileForce);

    m_scene->scheduleAction(
        [scene, worldProjectilePosition, worldProjectileForce, this]()
        {
            auto projectile = new Projectile(scene);
            projectile->moveTo(worldProjectilePosition);
            projectile->applyForce(worldProjectileForce);
            scene->addNode(projectile);
        }
    );
    m_accumulate = false;
    m_elapsedMilliseconds = 0;
}

void WeaponWagon::setAccumulate(bool accumulate)
{
    m_accumulate = accumulate;
}

void WeaponWagon::update(int elapsedMilliseconds)
{
    if(m_accumulate)
    {
        if(m_elapsedMilliseconds < 3000)
        {
            m_elapsedMilliseconds += elapsedMilliseconds;
        }

        m_force = QVector3D(0.f, m_elapsedMilliseconds/4, 1000 + m_elapsedMilliseconds * 2);
    }
    AbstractWagon::update(elapsedMilliseconds);
}

void WeaponWagon::render(QOpenGLFunctions& gl) const
{
    Program & program = **(ResourceManager::getInstance()->getProgram("basicShader"));
    Material & material = **(ResourceManager::getInstance()->getMaterial("base_Blue"));
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
    return 4.0f;
}

} //namespace terminus
