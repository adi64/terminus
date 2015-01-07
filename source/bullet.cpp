#include "bullet.h"

#include <QDebug>

#include <bullet/btBulletDynamicsCommon.h>

#include "scene.h"
#include "resources/resourcemanager.h"
#include "resources/geometry.h"
#include "resources/material.h"
#include "resources/program.h"

namespace terminus
{

Bullet::Bullet(Scene *scene)
    : AbstractGraphicsObject(scene)
{   
    auto shape = new btSphereShape(1.0);

    auto rotationQuaternion = btQuaternion(eulerAngles().x(), eulerAngles().y(), eulerAngles().z(), 1.0);
    auto positionVector = btVector3(position().x(), position().y(), position().z());

    auto motionState = new btDefaultMotionState(btTransform(rotationQuaternion, positionVector));

    auto mass = btScalar(1.0);

    auto inertia = btVector3(0.0, 0.0, 0.0);

    shape->calculateLocalInertia(mass, inertia);

    auto rigidBodyConstructionInfo = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, shape, inertia);

    m_bullet_rigidBody = std::unique_ptr<btRigidBody>(new btRigidBody(rigidBodyConstructionInfo));

    m_scene->bullet_world()->addRigidBody(m_bullet_rigidBody.get());
}

void Bullet::update(int elapsedMilliseconds)
{
    AbstractGraphicsObject::update(elapsedMilliseconds);

    m_ageInMilliseconds += elapsedMilliseconds;
    if(m_ageInMilliseconds > maxAgeInMilliseconds())
    {
        qDebug() << "bullet is old, please delete";
    }
}


void Bullet::render(QOpenGLFunctions& gl) const
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

unsigned int Bullet::maxAgeInMilliseconds() const
{
    return 5000;
}

} //namespace terminus
