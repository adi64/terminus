#include "projectile.h"

#include <QDebug>

#include <bullet/btBulletDynamicsCommon.h>

#include "scene.h"
#include "resources/resourcemanager.h"
#include "resources/geometry.h"
#include "resources/material.h"
#include "resources/program.h"

namespace terminus
{

Projectile::Projectile(std::shared_ptr<Scene> scene)
    : DynamicPhysicsObject(scene)
    , m_ageInMilliseconds(0)
{   
    auto myShape = new btSphereShape(1.0);
    m_bullet_rigidBody->setCollisionShape(myShape);
    m_bullet_collisionShape.reset(myShape);

    m_bullet_rigidBody->setMassProps(1.0f, btVector3(0.0f, 0.0f, 0.0f));

    m_scene->bullet_world()->addRigidBody(m_bullet_rigidBody.get());
}

void Projectile::update(int elapsedMilliseconds)
{
    DynamicPhysicsObject::update(elapsedMilliseconds);

    m_ageInMilliseconds += elapsedMilliseconds;
    if(m_ageInMilliseconds > maxAgeInMilliseconds())
    {
        // delete node

        // TODO FIXME this requires event synchronization,
        // otherwise it will mess up the Scene::update routine

        //m_scene->deleteNode(this);
        //delete(this);
    }
}

void Projectile::render(QOpenGLFunctions& gl) const
{
    // render terrain
    Program & program = **(ResourceManager::getInstance()->getProgram("basicShader"));
    Material & material = **(ResourceManager::getInstance()->getMaterial("base_Red"));
    Geometry & geometry = **(ResourceManager::getInstance()->getGeometry("base_Icosahedron"));

    program.bind();

    m_scene->camera().setMatrices(program, m_modelMatrix);
    material.setUniforms(program);
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
    geometry.setAttributes(program);

    geometry.draw(gl);

    program.release();
}

unsigned int Projectile::maxAgeInMilliseconds() const
{
    return 5000;
}

} //namespace terminus
