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
{
    auto myShape = new btBoxShape(btVector3(2.5, 1.0, 1.0));
    m_bullet_rigidBody->setCollisionShape(myShape);
    m_bullet_collisionShape.reset(myShape);

    m_bullet_rigidBody->setMassProps(1000.0f, btVector3(0.0f, 0.0f, 0.0f));

    m_scene->bullet_world()->addRigidBody(m_bullet_rigidBody.get());
}

void WeaponWagon::primaryAction()
{
    auto projectile = new Projectile(m_scene);
    projectile->setPosition(position() + QVector3D(0.0, 1.0, 2.2));
    projectile->applyForce(QVector3D(0.0, 100.0, 300.0));

    m_scene->addNode(projectile);
}

void WeaponWagon::render(QOpenGLFunctions& gl) const
{
    Program & program = **(ResourceManager::getInstance()->getProgram("basicShader"));
    Material & material = **(ResourceManager::getInstance()->getMaterial("base_Blue"));
    Geometry & geometry = **(ResourceManager::getInstance()->getGeometry("base_Wagon"));

    program.bind();

    m_scene->camera().setMatrices(program, m_modelMatrix);
    material.setUniforms(program);
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
    geometry.setAttributes(program);

    geometry.draw(gl);

    program.release();
}

float WeaponWagon::length() const
{
    return 5.0f;
}

} //namespace terminus
