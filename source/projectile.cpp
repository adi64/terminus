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

Projectile::Projectile(Scene *scene)
    : AbstractPhysicsObject(scene)
{   
    auto shape = new btSphereShape(1.0);

    auto rotationQuaternion = btQuaternion(0.0, 0.0, 0.0, 1.0);
    auto positionVector = btVector3(m_position.x(), m_position.y(), m_position.z());

    auto motionState = new btDefaultMotionState(btTransform(rotationQuaternion, positionVector));

    auto mass = btScalar(1.0);

    auto inertia = btVector3(0.0, 0.0, 0.0);

    shape->calculateLocalInertia(mass, inertia);

    auto rigidBodyConstructionInfo = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, shape, inertia);

    m_bullet_rigidBody = std::unique_ptr<btRigidBody>(new btRigidBody(rigidBodyConstructionInfo));

    m_scene->bullet_world()->addRigidBody(m_bullet_rigidBody.get());
}

void Projectile::update(int elapsedMilliseconds)
{
    AbstractPhysicsObject::update(elapsedMilliseconds);

    m_ageInMilliseconds += elapsedMilliseconds;
    if(m_ageInMilliseconds > maxAgeInMilliseconds())
    {
        // please delete
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
